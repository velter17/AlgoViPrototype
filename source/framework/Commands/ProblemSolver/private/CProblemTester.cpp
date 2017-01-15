/**
 * Project   Graviz
 *
 * @file     CProblemTester.cpp
 * @author   Dmytro Sadovyi
 * @date     10.12.2016
 */

#include "../CProblemTester.h"
#include "framework/Commands/ProblemSolver/checkers/CStraightForwardChecker.h"
#include "framework/Commands/ProblemSolver/checkers/CTestLibChecker.h"

namespace NCommand
{

CProblemTester::CProblemTester(const QStringList &args,
                               std::shared_ptr<CTestProvider> testProvider,
                               std::shared_ptr<CCompilerHandler> compilerHandler)
    : ITerminalCommand(args)
    , mTestProvider(testProvider)
    , mCompilerHandler(compilerHandler)
    , mTerminateFlag(false)
    , mAcceptedCounter(0)
    , mProblemSolverPtr(nullptr)
{
    mOptions.add_options()
        ("src,s", boost::program_options::value<std::string>()->required(), "Source code path")
        ("checker,c", boost::program_options::value<std::string>(), "Checker type")
        ("test,t", boost::program_options::value<std::string>(),
            "If no param - test whole test archive\n"
            "If -t 2 -> test second test from archive\n"
            "If -t 2-5 -> test from 2 to 5 tests from archive")
        ("verbose", boost::program_options::bool_switch()->default_value(false), "detailed report");
}

void CProblemTester::run()
{
    emit started();
    boost::program_options::variables_map vm;
    if(!readOptions(mArgs, vm))
    {
        emit finished(0);
        return;
    }

    if(vm.count("checker"))
    {
        mCheckerType = QString::fromStdString(vm["checker"].as<std::string>());
    }

    auto validateNum = [](const QString& str)
    {
        bool ret = true;
        for(char c : str.toStdString())
            ret &= isdigit(c);
        return ret;
    };

    if(vm.count("test"))
    {
        QString testNum = QString::fromStdString(vm["test"].as<std::string>());
        if(testNum.indexOf("-") == -1)
        {
            if(validateNum(testNum))
            {
                int test = testNum.toInt();
                if(test < 1 || test > mTestProvider->size())
                {
                    QString toEmit = testNum + " - index out of range\n";
                    if(mTestProvider->size() == 0)
                        toEmit += "Test archive is empty\n";
                    else
                        toEmit += "Valid range is [1.." + QString::number(mTestProvider->size()) + "]\n";
                    emit error(toEmit);
                    emit finished(1);
                    return;
                }
                else
                {
                    mTestFrom = testNum.toInt()-1;
                    mTestTo = mTestFrom;
                }
            }
            else
            {
                emit error(testNum + " is not a valid number");
                emit finished(1);
                return;
            }
        }
        else
        {
            QStringList range = testNum.split('-');
            if(range.size() != 2)
            {
                emit error(testNum + " is not valid number range");
                emit finished(1);
                return;
            }
            else
            {
                QString strFrom = *range.begin();
                QString strTo = *(range.begin()+1);
                if(!validateNum(strFrom) || strFrom.toInt() < 1 || strFrom.toInt() > mTestProvider->size())
                {
                    emit error(strFrom + " is not valid number");
                    emit finished(1);
                    return;
                }
                else if(!validateNum(strTo) || strTo.toInt() < 1 || strTo.toInt() > mTestProvider->size())
                {
                    emit error(strTo + " is not valid number");
                    emit finished(1);
                    return;
                }
                else
                {
                    int from = strFrom.toInt();
                    int to = strTo.toInt();
                    if(from > to)
                    {
                        emit error(testNum + " - invalid range. " + strFrom + " > " + strTo);
                        emit finished(1);
                        return;
                    }
                    else
                    {
                        mTestFrom = from-1;
                        mTestTo = to-1;
                    }
                }
            }
        }
    }
    else
    {
        mTestFrom = 0;
        mTestTo = mTestProvider->size()-1;
    }

    mSourceCodePath = QString::fromStdString(CFileSystem::getInstance().getFullPath(
                QString::fromStdString(vm["src"].as<std::string>())).string());
    mNeedDetails = vm["verbose"].as<bool>();

    if(!mCompilerHandler->isSourceCode(mSourceCodePath))
    {
        mCompilerHandler->addSourceCodePath(mSourceCodePath);
    }

    if(mCompilerHandler->isNeededCompilation(mSourceCodePath))
    {
        qDebug () << "neededCompilation";
        std::shared_ptr<QMetaObject::Connection> pconn(new QMetaObject::Connection);
        QMetaObject::Connection &conn = *pconn;
        conn = connect(mCompilerHandler.get(), &CCompilerHandler::finished,
                       [this, pconn, &conn](int code){
            qDebug () << "compilation was finished";
            if(code == 0)
            {
                testRunner(mTestFrom);
            }
            else
            {
                emit error(" [ Error ] problem with compilation. Exit.\n");
                emit finished(code);
            }
            disconnect(conn);
        });
        mCompilerHandler->performCompilation(mSourceCodePath, QStringList());
    }
    else
    {
        testRunner(mTestFrom);
    }
}

void CProblemTester::appendData(const QString &data)
{

}

void CProblemTester::terminate()
{
    qDebug () << "CProblemTester> terminate";
    if(mProblemSolverPtr != nullptr)
    {
        mProblemSolverPtr->terminate();
    }
    mTerminateFlag = true;
}

void CProblemTester::testRunner(int test)
{
    qDebug () << "testRunner> " << test;
    if(test > mTestTo || mTerminateFlag)
    {
        emit logHtml(" Result: " + QString::number(mAcceptedCounter) +
                " / " + QString::number(mTestTo-mTestFrom+1) + " - " +
                (mAcceptedCounter == mTestTo-mTestFrom+1 ?
                         "<font color=\"#9FFF3F\">Tests passed</font>" :
                         "<font color=\"#FF6F3F\">Partial solution</font>")
                + "<br>");
        emit finished(0);
        return;
    }

    CProblemSolver* problemSolver = new CProblemSolver(
                QStringList()
                    << "-s" << mSourceCodePath, mTestProvider);
    mProblemSolverPtr = problemSolver;
    problemSolver->init();
    problemSolver->setAppPath(mCompilerHandler->getAppPath(mSourceCodePath));
    connect(problemSolver, &CProblemSolver::started, [this, test, problemSolver](){
        qDebug () << "Testing of " << test+1 << " test was started";
        mOutputBuffer.clear();
        mErrorBuffer.clear();
        QMetaObject::invokeMethod(problemSolver, "appendData", Qt::QueuedConnection,
                                  Q_ARG(QString, mTestProvider->get(test).input));
    });
    connect(problemSolver, &CProblemSolver::finished,
                [this, test, problemSolver](int code){
        qDebug () << "finished with output: " << mOutputBuffer;
        qDebug () << "test " << test+1 << " finished with code " << code;
        emit logHtml(" test #" + QString::number(test+1) + ": " + checkResult(test, code));
        problemSolver->deleteLater();
        mProblemSolverPtr = nullptr;
        testRunner(test+1);
    });
    connect(problemSolver, &CProblemSolver::log, [this](const QString& log){
        mOutputBuffer += log;
    });
    connect(problemSolver, &CProblemSolver::error, [this](const QString& log){
        mErrorBuffer = log;
    });
    problemSolver->run();
}

QString CProblemTester::checkResult(int test, int returnCode)
{
    QString ret;
    if(returnCode == 0 && mErrorBuffer.isEmpty())
    {
        IProblemChecker *checker = nullptr;
        if(mCheckerType.isEmpty())
        {
            checker = new CStraightForwardChecker(
                QStringList()
                    << "--data" << mOutputBuffer
                    << "--answer" << mTestProvider->get(test).output);
        }
        else
        {
            checker = new CTestLibChecker(
                        QStringList()
                            << "--input" << mTestProvider->get(test).input
                            << "--answer" << mTestProvider->get(test).output
                            << "--output" << mOutputBuffer,
                        mCheckerType);
        }
        checker->run();
        if(checker->getResult() == TCheckerResult::Success)
        {
            ret = "<font color=green>OK</font>";
            ++mAcceptedCounter;
        }
        else if(checker->getResult() == TCheckerResult::Fail)
        {
            ret = "<font color=red>WA</font>";
        }
        else
        {
            ret = "Unexpected problems occured :(";
        }
        if(mNeedDetails)
        {
            ret += checker->details();
        }
        checker->deleteLater();
    }
    else
    {
        ret = "<font color=\"#CF8F18\">RE</font>";
        if(mNeedDetails)
        {
            if(mErrorBuffer.isEmpty())
            {
                ret += " ( Runtime error, exit with code " + QString::number(returnCode) + " )";
            }
            else
            {
                ret += " ( " + mErrorBuffer + " )";
            }
        }
    }
    ret += "<br>";
    return ret;
}


} // namespace NCommand
