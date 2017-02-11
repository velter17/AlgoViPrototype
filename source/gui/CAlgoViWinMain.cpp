/**
 * Project   AlgoVi
 *
 * @file     CAlgoViWinMain.cpp
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#include <QDebug>
#include <QProcess>
#include <QLabel>
#include <QThread>

#include "gui/CAlgoViWinMain.h"
#include "ui_CAlgoViWinMain.h"
#include "algovi/Types.h"

CAlgoViWinMain::CAlgoViWinMain(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CAlgoViWinMain)
{
    ui->setupUi(this);
    connect(ui->mTerminal, &NController::CTerminal::command,
            [this](NController::TTerminalCommandType type,
                   const QString& cmd){emit newCommand(type, cmd);});
}

CAlgoViWinMain::~CAlgoViWinMain()
{
    delete ui;
}


QGraphicsView *CAlgoViWinMain::getGraphicsView()
{
    return ui->mGraphicsView;
}

void CAlgoViWinMain::lock()
{
    ui->mTerminal->lock();
}

void CAlgoViWinMain::unlock()
{
    ui->mTerminal->unlock();
}

void CAlgoViWinMain::appMode()
{
   ui->mTerminal->appMode();
}

void CAlgoViWinMain::questionMode()
{
   ui->mTerminal->questionMode();
}

void CAlgoViWinMain::handleLog(QString msg)
{
    ui->mTerminal->appendOutput(msg);
}

void CAlgoViWinMain::handleError(QString msg)
{
    ui->mTerminal->appendError(msg);
}

void CAlgoViWinMain::handleLogHtml(QString msg)
{
    ui->mTerminal->appendOutputHtml(msg);
}

void CAlgoViWinMain::init()
{
}
