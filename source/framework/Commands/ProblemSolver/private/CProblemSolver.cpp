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

namespace NCommand
{

CProblemSolver::CProblemSolver(const QStringList &args)
    : ITerminalCommand(args)
{
    mOptions.add_options()
        ("src,s", boost::program_options::value<std::string>()->required(), "source code")
        ("flag,f", boost::program_options::value<std::vector<std::string>>(&mFlagParsed),
            "compilation flags\n"
            "use c++ -DVAL like -f VAL")
        ("input,i",boost::program_options::value<std::string>(), "input file")
        ("output,o", boost::program_options::value<std::string>(), "output file");
}

QString CProblemSolver::getSourceCodePath()
{
    return QString::fromStdString(mVarMap["src"].as<std::string>());
}

const QStringList& CProblemSolver::getCompilationFlags()
{
    return mFlags;
}

bool CProblemSolver::init()
{
    if(readOptions(mArgs, mVarMap))
    {
        for(const std::string& f : mFlagParsed)
        {
            mFlags << "-f" << QString::fromStdString(f);
        }
        return true;
    }
    else
    {
        return false;
    }
}

void CProblemSolver::run()
{
    qDebug () << "solve " << getSourceCodePath();

    mApp = new QProcess();
    connect(mApp, &QProcess::readyReadStandardError, [this](){
        emit error(mApp->readAllStandardError());
    });
    connect(mApp, &QProcess::readyReadStandardOutput, [this](){
        QString str = mApp->readAllStandardOutput();
        qDebug () << "output : " << str;
        //emit out(mApp->readAllStandardOutput());
        emit log(str);
    });
    connect(mApp, &QProcess::started, [this](){
        qDebug () << "started solver!!!";
        emit started();
    });
    connect(mApp, static_cast<void((QProcess::*)(int))>(&QProcess::finished), [this](int exitCode){
        qDebug () << "QProcess::finished";
        emit finished(exitCode);
       // mApp.reset();
    });
    //connect(mApp, &QProcess::errorOccurred, [](QProcess::ProcessError error){qDebug () << error;});
    mApp->start(QString("stdbuf -o 0 ") + "/home/dsadovyi/Code/app", QProcess::Unbuffered | QProcess::ReadWrite);
//    connect(thread, &QThread::started, [this](){
//        mApp->start(QString("stdbuf -o 0 ") + mSettings.getSolverAppPath(), QProcess::Unbuffered | QProcess::ReadWrite);
//        qDebug () << "process pid is  " << mApp->pid();
//        //mApp->waitForFinished();
//    });
//    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
//    thread->start();
    mApp->waitForStarted();
//    appendData(args);
 //   mApp->waitForFinished();
}

void CProblemSolver::terminate()
{
    mApp->terminate();
}

void CProblemSolver::appendData(const QString &data)
{
    assert(0 != mApp);
    if(data.isEmpty())
    {
        return;
    }
    qDebug () << "append(write) " << data;
    mApp->write(data.toLocal8Bit());
    mApp->write("\n");
}

} // namespace NCommand
