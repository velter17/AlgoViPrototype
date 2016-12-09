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
        ("input,i",boost::program_options::value<std::string>(), "input file")
        ("output,o", boost::program_options::value<std::string>(), "output file")
        ("test-save,t", boost::program_options::bool_switch()->default_value(false), "Post saving test to archive")
        ("<", "Use to redirect input from archive-test");
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
    });
    mApp->start(QString("stdbuf -o 0 ") + "/home/dsadovyi/Coding/app", QProcess::Unbuffered | QProcess::ReadWrite);
    mApp->waitForStarted();
    if(mTestToExecuteFlag)
    {
        this->appendData(mTestProvider->get(mTestToExecute).input);
    }
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
