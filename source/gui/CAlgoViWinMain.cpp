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
    ui->mGraphicsView->setMaximumHeight(16777215);
    ui->ioWidget->setMaximumHeight(16777215);

    connect(ui->mTerminal, &NController::CTerminal::command,
            [this](NController::TTerminalCommandType type,
                   const QString& cmd){emit newCommand(type, cmd);});
    connect(ui->ioOkButton, &QPushButton::clicked, [this]{
        emit ioOkButtonPressed();
    });
    connect(ui->ioCancelButton, &QPushButton::clicked, [this]{
        emit ioCancelButtonPressed();
    });

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

void CAlgoViWinMain::showGraphicScene(bool val)
{
    ui->mGraphicsView->setVisible(val);
}

void CAlgoViWinMain::showIoWin(bool val)
{
    ui->ioWidget->setVisible(val);
    if(!val)
    {
        ui->inputTextEdit->clear();
        ui->outputTextEdit->clear();
        ui->splitter->restoreState(mSplitterState);
    }
    else
    {
        mSplitterState = ui->splitter->saveState();
    }
}

QPair<QString, QString> CAlgoViWinMain::getIoData()
{
    return QPair<QString, QString>(ui->inputTextEdit->toPlainText(), ui->outputTextEdit->toPlainText());
}

void CAlgoViWinMain::setIoData(const QString &input, const QString &output)
{
    ui->inputTextEdit->append(input);
    ui->outputTextEdit->append(output);
}

void CAlgoViWinMain::init()
{
}
