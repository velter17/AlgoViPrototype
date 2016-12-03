/**
 * Project   Graviz
 *
 * @file     CGravizWinMain.cpp
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#include <QDebug>
#include <QProcess>
#include <QLabel>
#include <QThread>

#include "gui/CGravizWinMain.h"
#include "ui_CGravizWinMain.h"
#include "graviz/Types.h"

CGravizWinMain::CGravizWinMain(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CGravizWinMain)
{
    ui->setupUi(this);
    connect(ui->mTerminal, &NController::CTerminal::command,
            [this](NController::TTerminalCommandType type,
                   const QString& cmd){emit newCommand(type, cmd);});
}

CGravizWinMain::~CGravizWinMain()
{
    delete ui;
}

void CGravizWinMain::setAlgoLabel(const QString &title, const QString &value)
{
    if(mAlgoLabels.find(title) == mAlgoLabels.end())
    {
        mAlgoLabels.insert(title, std::shared_ptr<QLabel>(new QLabel(value)));
        int row = ui->mAlgoLayout->rowCount();
        ui->mAlgoLayout->addWidget(new QLabel(title), row, 0);
        ui->mAlgoLayout->addWidget(mAlgoLabels[title].get(), row, 1);
    }
    mAlgoLabels[title]->setText(value);
}

QGraphicsView *CGravizWinMain::getGraphicsView()
{
    return ui->mGraphicsView;
}

void CGravizWinMain::lock()
{
    ui->mTerminal->lock();
}

void CGravizWinMain::unlock()
{
    ui->mTerminal->unlock();
}

void CGravizWinMain::appMode()
{
    ui->mTerminal->appMode();
}

void CGravizWinMain::handleLog(QString msg)
{
    ui->mTerminal->appendOutput(msg);
}

void CGravizWinMain::handleError(QString msg)
{
    ui->mTerminal->appendError(msg);
}

void CGravizWinMain::init()
{
}
