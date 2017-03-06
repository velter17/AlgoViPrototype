/**
 * Project   AlgoVi
 *
 * @file     CProblemTester.cpp
 * @author   Dmytro Sadovyi
 * @date     10.12.2016
 */

#include "../CProblemTesterInteractive.h"
#include "framework/Commands/ProblemSolver/checkers/CStraightForwardChecker.h"
#include "framework/Commands/ProblemSolver/checkers/CInteractorChecker.h"
#include "framework/Commands/ProblemSolver/checkers/CTestLibChecker.h"

namespace NCommand
{

CProblemTesterInteractive::CProblemTesterInteractive(
      const QStringList &args,
      std::shared_ptr<CTestProvider> testProvider,
      std::shared_ptr<CCompilerHandler> compilerHandler)
    : ITerminalCommand(args)
    , mTestProvider(testProvider)
    , mCompilerHandler(compilerHandler)
    , mTerminateFlag(false)
    , mAcceptedCounter(0)
    , mProblemSolverPtr(nullptr)
    , mInteractorPtr(nullptr)
{
    mOptions.add_options()
        ("src,s", boost::program_options::value<std::string>()->required(), "Source code path")
        ("checker,c", boost::program_options::value<std::string>()->required(),
            "Checker types:\n"
            "* No argument - default system checker\n"
            "* [testlib builtin checker] - see --manual for more details\n"
            "* Path to checker code\n")
        ("interactor,i", boost::program_options::value<std::string>()->required(), "Interactor code path")
        ("test,t", boost::program_options::value<std::string>(),
            "If no param - test whole test archive\n"
            "If -t 2 -> test second test from archive\n"
            "If -t 2-5 -> test from 2 to 5 tests from archive")
        ("verbose", boost::program_options::bool_switch()->default_value(false), "detailed report")
        ("full-protocol", boost::program_options::bool_switch()->default_value(false), "full interaction protocol");
}

void CProblemTesterInteractive::run()
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
        mCustomChecker = CFileSystem::getInstance().exists(mCheckerType);
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
    mDetailedReport = vm["verbose"].as<bool>();
    mFullProtocol = vm["full-protocol"].as<bool>();
    mInteractorCodePath = QString::fromStdString(CFileSystem::getInstance().getFullPath(
                                                 QString::fromStdString(vm["interactor"].as<std::string>())).string());
    mCheckerCodePath = QString::fromStdString(CFileSystem::getInstance().getFullPath(
                                                 QString::fromStdString(vm["checker"].as<std::string>())).string());

    std::vector<QString> codes {mSourceCodePath, mInteractorCodePath};
    if(mCustomChecker)
    {
       codes.push_back(mCheckerCodePath);
    }

    compile(codes, 0);
}

void CProblemTesterInteractive::appendData(const QString &data)
{

}

void CProblemTesterInteractive::terminate()
{
    qDebug () << "CProblemTester> terminate";
    if(mProblemSolverPtr != nullptr)
    {
        mProblemSolverPtr->terminate();
    }
    mTerminateFlag = true;
}

QString CProblemTesterInteractive::getManualMessage()
{
   return ITerminalCommand::getManualMessage();
}

void CProblemTesterInteractive::compile(const std::vector<QString> codes, size_t code_idx)
{
   if(code_idx == codes.size())
   {
       runTester(mTestFrom);
       return;
   }
   if(!codes[code_idx].isEmpty() && !mCompilerHandler->isSourceCode(codes[code_idx]))
   {
       mCompilerHandler->addSourceCodePath(codes[code_idx]);
   }

   if(!codes[code_idx].isEmpty() && mCompilerHandler->isNeededCompilation(codes[code_idx]))
   {
       qDebug () << "neededCompilation";
       std::shared_ptr<QMetaObject::Connection> pconn(new QMetaObject::Connection);
       QMetaObject::Connection &conn = *pconn;
       conn = connect(mCompilerHandler.get(), &CCompilerHandler::finished,
                      [this, pconn, &conn, code_idx, codes](int code){
           qDebug () << "compilation was finished";
           if(code == 0)
           {
               compile(codes, code_idx+1);
           }
           else
           {
               emit error(" [ Error ] problem with compilation. Exit.\n");
               emit finished(code);
           }
           disconnect(conn);
       });
       mCompilerHandler->performCompilation(codes[code_idx], QStringList());
   }
   else
   {
       compile(codes, code_idx+1);
   }
}

void CProblemTesterInteractive::runTester(int test)
{
   qDebug () << "CProblemTesterInteractive> testRunner> " << test;
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

   std::ofstream file((mDir.path() + "input.txt").toStdString());
   file << mTestProvider->get(test).input.toStdString();
   file.close();

   file.open((mDir.path() + "answer.txt").toStdString());
   file << mTestProvider->get(test).output.toStdString();
   file.close();

   mInteractorPtr = new CProblemSolver(
            QStringList() << "-s" << mInteractorCodePath
                      << "--arg" << mDir.path() + "input.txt"
                      << "--arg" << mDir.path() + "output.txt"
                      << "--arg" << mDir.path() + "answer.txt",
            mTestProvider);
   mInteractorPtr->init();
   mInteractorPtr->setAppPath(mCompilerHandler->getAppPath(mInteractorCodePath));


   connect(problemSolver, &CProblemSolver::started, [this, test, problemSolver](){
       qDebug () << "Testing of " << test+1 << " test was started";
       mOutputBuffer.clear();
       mErrorBuffer.clear();
   });
   connect(problemSolver, &CProblemSolver::finished,
               [this, test, problemSolver](int code){
       qDebug () << "problemSolver finished with code " << code;
//       qDebug () << "test " << test+1 << " finished with code " << code;
//       emit logHtml(" test #" + QString::number(test+1) + ": " + checkResult(test, code));
       problemSolver->deleteLater();
       mProblemSolverPtr = nullptr;
       mInteractorPtr->sendEOF();
//       testRunner(test+1);
   });
   connect(problemSolver, &CProblemSolver::log, [this](const QString& log){
//       mOutputBuffer += log;
      //qDebug () << "TesterInteractor> problemSolver log: " << log;
      mInteractionProtocol.push_back({false, log});
      if(mInteractorPtr != nullptr)
          mInteractorPtr->appendData(log);
      else
          mProblemSolverPtr->terminate();
   });
   connect(problemSolver, &CProblemSolver::error, [this](const QString& log){
      //qDebug () << "TesterInteractor> problemSolver error: " << log;
//       mErrorBuffer = log;
   });
   connect(mInteractorPtr, &CProblemSolver::log, [this](const QString& log){
      //qDebug () << "TesterInteractor> interator log: " << log;
      mInteractionProtocol.push_back({true, log});
      if(mProblemSolverPtr != nullptr)
          mProblemSolverPtr->appendData(log);
   });
   connect(mInteractorPtr, &CProblemSolver::error, [this](const QString& log){
      //qDebug () << "TesterInteractor> interator log: " << log;
      mErrorBuffer = log;
   });
   connect(mInteractorPtr, &CProblemSolver::finished, [this, test](int code){
      qDebug () << "TesterInteractor> finished";
      mInteractorPtr->deleteLater();
      mInteractorPtr = nullptr;
      if(mProblemSolverPtr != nullptr)
      {
         mProblemSolverPtr->terminate();
      }
      emit logHtml(" test #" + QString::number(test+1) + ": " + checkResult(test, code));
      runTester(test+1);
   });
   mInteractionProtocol.clear();
   problemSolver->run();
   mInteractorPtr->run();
}

QString CProblemTesterInteractive::checkResult(int test, int returnCode)
{
    QString ret;
    if(returnCode == 0 && mErrorBuffer.isEmpty())
    {
//        IProblemChecker *checker = new CInteractorChecker(
//                            QStringList()
//                                << "--input" << mDir.path() + "input.txt"
//                                << "--answer" << mDir.path() + "answer.txt"
//                                << "--output" << mDir.path() + "output.txt",
//                            mCompilerHandler->getAppPath(mCheckerCodePath));
//        checker->run();
       IProblemChecker *checker;
       if(mCheckerType.isEmpty())
       {
           checker = new CStraightForwardChecker(
               QStringList()
                   << "--data" << mOutputBuffer
                   << "--answer" << mTestProvider->get(test).output);
       }
       else
       {
           if(!mCustomChecker)
           {
               checker = new CInteractorChecker(
                        QStringList()
                            << "--input" << mDir.path() + "input.txt"
                            << "--answer" << mDir.path() + "answer.txt"
                            << "--output" << mDir.path() + "output.txt",
                           "checkers/" + mCheckerType);
           }
           else
           {
              checker = new CInteractorChecker(
                           QStringList()
                               << "--input" << mDir.path() + "input.txt"
                               << "--answer" << mDir.path() + "answer.txt"
                               << "--output" << mDir.path() + "output.txt",
                           mCompilerHandler->getAppPath(mCheckerCodePath));
           }
           checker->run();
       }
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
            ret = "<font color=\"#CF8FA8\">Fail</font>";
        }
        if(mFullProtocol && checker->getResult() != TCheckerResult::Success)
        {
            ret += "<br>======---Interaction protocol---=======<br>";
            for(std::size_t i = 0; i < mInteractionProtocol.size(); ++i)
            {

                if(mInteractionProtocol[i].first)
                    ret += "<font color=green>Interactor: </font>";
                else
                    ret += "<font color=red>Solution: </font>";
                ret += mInteractionProtocol[i].second + "<br>";
            }
            ret += "======---====================---=======<br>";
        }
        if(mDetailedReport)
        {
            ret += checker->details();
        }
        checker->deleteLater();
    }
    else
    {
        ret = "<font color=\"#CF8F18\">RE</font>";
        if(mFullProtocol)
        {
            ret += "<br>======---Interaction protocol---=======<br>";
            for(std::size_t i = 0; i < mInteractionProtocol.size(); ++i)
            {

                if(mInteractionProtocol[i].first)
                    ret += "<font color=green>Interactor: </font>";
                else
                    ret += "<font color=red>Solution: </font>";
                ret += mInteractionProtocol[i].second + "<br>";
            }
            ret += "======---====================---=======<br>";
        }
        if(mDetailedReport)
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
