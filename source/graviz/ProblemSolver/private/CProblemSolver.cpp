/**
 * Project   Graviz
 *
 * @file     CProblemSolver.cpp
 * @author   Dmytro Sadovyi
 * @date     04.11.2016
 */

#include <cassert>
#include <memory>
#include <QProcess>
#include <QThread>
#include <QDebug>

#include "../CProblemSolver.h"

namespace NGraviz
{

CProblemSolver::CProblemSolver(const CSolverSettings &settings)
    : mSettings(settings)
    , mSettingsTimestamp(0)
    , mSettingsUpdatedTimestamp(0)
{

}

void CProblemSolver::solve(QString inputData)
{
    qDebug () << "solve " << mSettings.getSolverAppPath();
    if(mSettings.getSolverAppPath().isEmpty())
    {
        return;
    }

    mApp = new QProcess();
    //mApp.reset(new QProcess());
//    QThread *thread = new QThread;
//    mApp->moveToThread(thread);
    connect(mApp, &QProcess::readyReadStandardError, [this](){
        emit err(mApp->readAllStandardError());
    });
    connect(mApp, &QProcess::readyReadStandardOutput, [this](){
        QString str = mApp->readAllStandardOutput();
        qDebug () << "output : " << str;
        //emit out(mApp->readAllStandardOutput());
        emit out(str);
    });
    connect(mApp, &QProcess::started, [this](){
        qDebug () << "started solver!!!";
        emit started();
    });
    connect(mApp, static_cast<void((QProcess::*)(int))>(&QProcess::finished), [this](int exitCode){
        qDebug () << "QProcess::finished";
        emit finished(exitCode, "Finished");
       // mApp.reset();
    });
    connect(mApp, &QProcess::errorOccurred, [](QProcess::ProcessError error){qDebug () << error;});
    mApp->start(QString("stdbuf -o 0 ") + mSettings.getSolverAppPath(), QProcess::Unbuffered | QProcess::ReadWrite);
//    connect(thread, &QThread::started, [this](){
//        mApp->start(QString("stdbuf -o 0 ") + mSettings.getSolverAppPath(), QProcess::Unbuffered | QProcess::ReadWrite);
//        qDebug () << "process pid is  " << mApp->pid();
//        //mApp->waitForFinished();
//    });
//    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
//    thread->start();
    mApp->waitForStarted();
    appendData(inputData);
 //   mApp->waitForFinished();
}

void CProblemSolver::terminate()
{
    mApp->terminate();
}

void CProblemSolver::appendData(QString data)
{
    assert(mSettings.getType() == TProblemSolverType::Interactive);
    assert(0 != mApp);
    if(data.isEmpty())
    {
        return;
    }
    qDebug () << "append(write) " << data;
    mApp->write(data.toLocal8Bit());
    mApp->write("\n");
}

CSolverSettings CProblemSolver::getSettings() const
{
    return mSettings;
}

void CProblemSolver::setSettings(const CSolverSettings &settings)
{
    mSettings = settings;
    ++mSettingsTimestamp;
}


} // namespace NGraviz
