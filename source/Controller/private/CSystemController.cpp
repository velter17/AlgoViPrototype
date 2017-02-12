/**
 * Project   AlgoVi
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

CSystemController::CSystemController(std::shared_ptr<CAlgoViWinMain> GUI)
    : mGUI(GUI)
    , mView(nullptr)
    , mModel(nullptr)
{
    connect(mGUI.get(), SIGNAL(newCommand(NController::TTerminalCommandType,QString)),
            this, SLOT(handleCommand(NController::TTerminalCommandType, QString)));

    GUI->showGraphicScene(false);
    GUI->showIoWin(false);

    connect(mGUI.get(), &CAlgoViWinMain::ioOkButtonPressed, [this](){
        QString input = mGUI->getIoData().first;
        QString output = mGUI->getIoData().second;
        emit ioOkButtonPressed(input, output);
    });
    connect(mGUI.get(), &CAlgoViWinMain::ioCancelButtonPressed, [this](){
        emit ioCancelButtonPressed();
    });
}

void CSystemController::setModel(std::shared_ptr<NAlgoVi::CAlgoViSystem> model)
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

void CSystemController::handleLogHtml(QString msg)
{
    qDebug () << "CSystemController> handleError " << msg;
    mGUI->handleLogHtml(msg);
}

void CSystemController::unlock()
{
    mGUI->unlock();
}

void CSystemController::setAppMode()
{
   mGUI->appMode();
}

void CSystemController::setQuestionMode()
{
    mGUI->questionMode();
}

void CSystemController::exit()
{
    mGUI->close();
}

void CSystemController::showGraphicScene(bool val)
{
    mGUI->showGraphicScene(val);
}

void CSystemController::showIoWin(bool val)
{
    mGUI->showIoWin(val);
}

QPair<QString, QString> CSystemController::getIoData()
{
    return mGUI->getIoData();
}

void CSystemController::setIoData(const QString &input, const QString &output)
{
    mGUI->setIoData(input, output);
}

} // namespace NController
