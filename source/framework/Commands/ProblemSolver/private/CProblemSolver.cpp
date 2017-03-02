/**
 * Project   AlgoVi
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


#include "framework/Commands/CFileSystem.h"
#include "../CProblemSolver.h"

namespace NCommand
{

CProblemSolver::CProblemSolver(const QStringList &args, std::shared_ptr<CTestProvider> testProvider)
    : ITerminalCommand(args)
    , mTestProvider(testProvider)
    , mTestToExecuteFlag(false)
{
    mOptions.add_options()
        ("src,s", boost::program_options::value<std::string>()->required(), "source code")
        ("flag,f", boost::program_options::value<std::vector<std::string>>(&mFlagParsed),
            "compilation flags\n"
            "use c++ -DVAL like -f VAL")
        ("arg", boost::program_options::value<std::vector<std::string>>(&mArgsForApp), "specific args for app")
        ("input,i",boost::program_options::value<std::string>(), "input file")
        ("output,o", boost::program_options::value<std::string>(), "output file")
        ("test-save,t", boost::program_options::bool_switch()->default_value(false), "Post saving test to archive");
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
    if(mArgs.size() > 2 && *(mArgs.end()-2) == "<")
    {
        QString testNum = mArgs.back();
        mArgs.pop_back();
        mArgs.pop_back();
        auto validateNum = [](const QString& str)
        {
            bool ret = true;
            for(char c : str.toStdString())
                ret &= isdigit(c);
            return ret;
        };
        if(validateNum(testNum) && testNum.toInt() > 0 && testNum.toInt() <= mTestProvider->size())
        {
            mTestToExecuteFlag = true;
            mTestToExecute = testNum.toInt()-1;
        }
        else
        {
            if(!validateNum(testNum))
                emit error(testNum + " - invalid test num");
            else
                emit error(testNum + " - invalid test num (index out of range)");
            return false;
        }
    }
    if(readOptions(mArgs, mVarMap))
    {
        for(const std::string& f : mFlagParsed)
        {
            mFlags << "-f" << QString::fromStdString(f);
        }
        for(const std::string& arg : mArgsForApp)
        {
           mArgsForAppList << QString::fromStdString(arg);
        }
        if(mTestToExecuteFlag && mVarMap.count("input"))
        {
            emit error(" [ Error ] Input-file and test in same time is incorrect");
            return false;
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool CProblemSolver::saveTestFlag()
{
    return mVarMap["test-save"].as<bool>();
}

void CProblemSolver::setAppPath(const QString &path)
{
    mAppPath = path;
}

void CProblemSolver::run()
{
    qDebug () << "solve " << getSourceCodePath();

    mApp = new QProcess();
    connect(mApp, &QProcess::readyReadStandardError, [this](){
        QString str = mApp->readAllStandardError();
        qDebug () << "error : " << str;
        emit error(str);
    });
    connect(mApp, &QProcess::readyReadStandardOutput, [this](){
        QString str = mApp->readAllStandardOutput();
//        qDebug () << "output : " << str;
        if(mVarMap.count("output"))
        {
            mOutputFile << str.toStdString();
        }
        else
        {
            emit log(str);
        }
    });
    connect(mApp, &QProcess::started, [this](){
        qDebug () << "started solver!!!";
        emit started();
    });
    connect(mApp, static_cast<void((QProcess::*)(int, QProcess::ExitStatus))>(&QProcess::finished),
            [this](int exitCode, QProcess::ExitStatus exitStatus)
    {
        qDebug () << "QProcess::finished";
        if(exitStatus == QProcess::CrashExit)
        {
            exitCode = 1;
            emit error(mApp->errorString());
        }
        emit finished(exitCode);
    });

    if(mVarMap.count("output"))
    {
        std::string filePath = CFileSystem::getInstance().getFullPath(
                    QString::fromStdString(mVarMap["output"].as<std::string>())).string();
        mOutputFile.open(filePath);
    }
    qDebug () << "args for app " << getSourceCodePath() << " are " << mArgsForAppList;
#ifdef WIN_TARGET
    mApp->start(mAppPath, QProcess::Unbuffered | QProcess::ReadWrite);
#else
    mApp->start(/*QString("stdbuf -o 0 ") + */mAppPath, mArgsForAppList, QProcess::Unbuffered | QProcess::ReadWrite);
#endif
    mApp->waitForStarted();
    if(mTestToExecuteFlag)
    {
        this->appendData(mTestProvider->get(mTestToExecute).input);
        mApp->closeWriteChannel();
    }
    else if(mVarMap.count("input"))
    {
        std::string filePath = CFileSystem::getInstance().getFullPath(
                    QString::fromStdString(mVarMap["input"].as<std::string>())).string();
        std::ifstream  file(filePath);
        std::string buffer;
        while(std::getline(file, buffer))
        {
            this->appendData(QString::fromStdString(buffer) + "\n");
        }
        mApp->closeWriteChannel();
    }
}

void CProblemSolver::terminate()
{
    mApp->terminate();
}

void CProblemSolver::sendEOF()
{
    assert(0 != mApp);
    mApp->closeWriteChannel();
}

void CProblemSolver::appendData(const QString &data)
{
    assert(0 != mApp);
    if(data.isEmpty())
    {
        return;
    }
    mApp->write(data.toLocal8Bit());
    mApp->write("\n");
}

} // namespace NCommand
