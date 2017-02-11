/**
 * Project   AlgoVi
 *
 * @file     CSystemCmd.cpp
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#include <QTimer>

#include "framework/Commands/system/CSystemCmd.h"
#include "framework/settings/CCommandSettings.h"

namespace NCommand
{

CSystemCmd::CSystemCmd(QStringList args)
    : ITerminalCommand(args)
{

}

void CSystemCmd::run()
{
    emit started();

    boost::program_options::variables_map vm;
//    if(!readOptions(mArgs, vm))
//    {
//        emit finished(-1);
//        return;
//    }

    mProc = new QProcess();
    connect(mProc, &QProcess::readyReadStandardError, [this](){
        emit error(mProc->readAllStandardError());
    });
    connect(mProc, &QProcess::readyReadStandardOutput, [this](){
        emit log(mProc->readAllStandardOutput());
    });
    mProc->setWorkingDirectory(mDirectory);
    mProc->readChannelFinished();
    QTimer *timer = new QTimer();
    timer->setInterval(mTime);
    connect(timer, SIGNAL(timeout()), mProc, SLOT(kill()));
    connect(mProc, SIGNAL(started()), timer, SLOT(start()));
    connect(mProc, static_cast<void(QProcess::*)(int)>(&QProcess::finished),
            [this, timer](int code)
    {
        qDebug () << "mProc finished";
        timer->deleteLater();
        emit finished(code);
        mProc->deleteLater();
    });
    QString app = *mArgs.begin();
    mArgs.erase(mArgs.begin());
    if(NSettings::CCommandSettings::getInstance().isCommand(app))
    {
        qDebug () << "old app is " << app;
        app = NSettings::CCommandSettings::getInstance().getCommand(app);
        qDebug () << "new app = " << app;
    }
#ifdef WIN_TARGET
    qDebug () << app << " " << mArgs;
    mProc->start(app, QStringList() << mArgs);
#else
    for(QString &s : mArgs)
        app += " " + s;
    mProc->start("bash", QStringList() << "-c" << app);
#endif
//    mProc->waitForFinished();
//    qDebug () << "mProc finished";
}

void CSystemCmd::terminate()
{
    mProc->terminate();
}

void CSystemCmd::appendData(const QString &data)
{
    mProc->write(data.toLocal8Bit());
    mProc->write("\n");
}

} // namespace NCommand
