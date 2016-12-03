/**
 * Project   Graviz
 *
 * @file     CSystemController.cpp
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#include <QGraphicsView>
#include <QDebug>

#include "Controller/CSystemController.h"

namespace NController
{

CSystemController::CSystemController(std::shared_ptr<CGravizWinMain> GUI)
    : mGUI(GUI)
    , mView(nullptr)
    , mModel(nullptr)
{
    mGUI->setAlgoLabel("Algorithm",     "not specified");
    mGUI->setAlgoLabel("Type",          "not specified");
    mGUI->setAlgoLabel("InputType",     "not specified");
    mGUI->setAlgoLabel("OutputType",    "not specified");
    connect(mGUI.get(), SIGNAL(newCommand(NController::TTerminalCommandType,QString)),
            this, SLOT(handleCommand(NController::TTerminalCommandType, QString)));
}

void CSystemController::setModel(std::shared_ptr<NGraviz::CGravizSystem> model)
{
    mModel = model;
}

void CSystemController::setView(std::shared_ptr<NView::CGraphicView> view)
{
    mView = view;
    mGUI->getGraphicsView()->setScene(mView.get());
}

void CSystemController::handleCommand(NController::TTerminalCommandType type, const QString &cmd)
{
    qDebug () << "CSystemController> handleCommand " << cmd;
    assert(0 != mModel);
//    mGUI->lock();
    mModel->handleCommand(type, cmd);
}

void CSystemController::handleLog(QString msg)
{
    qDebug () << "CSystemController> handleLog " << msg;
    mGUI->handleLog(msg);
}

void CSystemController::handleError(QString msg)
{
    qDebug () << "CSystemController> handleError " << msg;
    mGUI->handleError(msg);
}

void CSystemController::unlock()
{
    mGUI->unlock();
}

void CSystemController::setAppMode()
{
    mGUI->appMode();
}

} // namespace NController
