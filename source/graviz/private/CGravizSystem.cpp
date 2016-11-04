/**
 * Project   Graviz
 *
 * @file     CGravizSystem.cpp
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#include <QProcess>
#include <QDebug>

#include "graviz/CGravizSystem.h"

namespace NGraviz
{

CGravizSystem::CGravizSystem(std::shared_ptr<NController::CSystemController> controll)
   : mView(new NView::CGraphicView())
   , mController(controll)
   , mMode(TSystemMode::Interaction)
   , mCommandHandler(new CCommandHandler(this))
{
    mController->setModel(std::shared_ptr<CGravizSystem>(this));
    mController->setView(mView);
    connect(mCommandHandler.get(), &CCommandHandler::endCommand,
            [this]{mController->unlock();});
    connect(mCommandHandler.get(), &CCommandHandler::log,
            [this](QString msg){mController->handleLog(msg);});
    connect(mCommandHandler.get(), &CCommandHandler::error,
            [this](QString msg){mController->handleError(msg);});
    connect(mCommandHandler.get(), &CCommandHandler::logHtml,
            [this](QString msg){mController->handleLogHtml(msg);});
}

void CGravizSystem::handleCommand(const QString &cmd)
{
    qDebug () << "CGravizSystem> handleCommand " << cmd;
    mCommandHandler->handle(cmd);
}

void CGravizSystem::setMode(TSystemMode mode)
{
    mMode = mode;
}

} // namespace NGraviz
