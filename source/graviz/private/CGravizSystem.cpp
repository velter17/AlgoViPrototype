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
{
    mController->setModel(std::shared_ptr<CGravizSystem>(this));
    mController->setView(mView);
}


void CGravizSystem::setMode(TSystemMode mode)
{
    mMode = mode;
}

} // namespace NGraviz
