/**
 * Project   Graviz
 *
 * @file     CGravizSystem.cpp
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#include <QProcess>
#include <QDebug>
#include <QThread>
#include <memory>

#include "graviz/CGravizSystem.h"
#include "framework/Commands/system/CCompiler.h"
#include "graviz/settings/CSolverSettings.h"
#include "framework/Commands/ProblemSolver/CTestCommand.h"
#include "framework/Commands/ProblemSolver/ProblemParser/CCodeforcesParser.h"
#include "framework/Commands/ProblemSolver/CProblemTester.h"

namespace NGraviz
{

CGravizSystem::CGravizSystem(std::shared_ptr<NController::CSystemController> controll)
   : mView(new NView::CGraphicView())
   , mController(controll)
   , mMode(TSystemMode::Default)
   , mCommandHandler(new CCommandHandler(this))
   , mCompilerHandler(new NCommand::CCompilerHandler())
   , mProblemSolver(nullptr)
   , mTestProvider(new NCommand::CTestProvider())
{
    mController->setModel(std::shared_ptr<CGravizSystem>(this));
    mController->setView(mView);

    CSolverSettings::getInstance()
            .setTimeLimit(100000)
            .setType(TProblemSolverType::Interactive);

    connect(&NCommand::CFileSystem::getInstance(), &NCommand::CFileSystem::error, [this](const QString &msg){
        mController->handleError(msg);
    });

    QThread* compilerHandlerThread = new QThread();
    mCompilerHandler->moveToThread(compilerHandlerThread);
    connect(mCompilerHandler.get(), &NCommand::CCompilerHandler::out, [this](const QString& msg){
        mController->handleLog(msg);
    });
    connect(mCompilerHandler.get(), &NCommand::CCompilerHandler::err, [this](const QString& msg){
        mController->handleError(msg);
    });
    connect(mCompilerHandler.get(), SIGNAL(destroyed(QObject*)), compilerHandlerThread, SLOT(deleteLater()));
    compilerHandlerThread->start();
}

void CGravizSystem::handleCommand(NController::TTerminalCommandType type, const QString &cmd)
{
    qDebug () << "CGravizSystem> handleCommand " << cmd;
    if(mMode == TSystemMode::InProcess)
    {
        qDebug () << "append data " << cmd;
        if(cmd == "^C")
        {
            QMetaObject::invokeMethod(mProblemSolver,"terminate", Qt::QueuedConnection);
        }
        else
        {
            mInputBuffer += cmd + "\n";
            QMetaObject::invokeMethod(mProblemSolver,"appendData", Qt::QueuedConnection,
                                      Q_ARG(QString, cmd));
        }
    }
    else if(mMode == TSystemMode::WaitForAnswer)
    {
        mQuestioner->appendData(cmd);
    }
    else
    {
        mCommandHandler->handle(cmd);
    }
}


void CGravizSystem::setMode(TSystemMode mode)
{
    mMode = mode;
}


template <TGravizCommand command>
void CGravizSystem::handleFuncRegistrator()
{
    mCommandHandlers[(size_t)command] = &CGravizSystem::handle<command>;
    handleFuncRegistrator<(TGravizCommand)((size_t)command+1)>();
}

template <>
void CGravizSystem::handleFuncRegistrator<TGravizCommand::Total>()
{

}

template <>
void CGravizSystem::handle<TGravizCommand::RunSolver>(const QStringList &args)
{
    qDebug () << "CGravizSystem> RunSolver " << args;
    setMode(TSystemMode::InProcess);
    //mProblemSolver.reset(new NCommand::CProblemSolver(args));
    mProblemSolver = new NCommand::CProblemSolver(args, mTestProvider);
    connect(mProblemSolver, &NCommand::CProblemSolver::log, [this](const QString& log){
        //mController->handleLog(log);
        QMetaObject::invokeMethod(mController.get(), "handleLog", Qt::QueuedConnection,
                                  Q_ARG(QString, log));
        mOutputBuffer += log;
    });
    connect(mProblemSolver, &NCommand::CProblemSolver::error, [this](const QString& log){
        //mController->handleError(log);
        QMetaObject::invokeMethod(mController.get(), "handleError", Qt::QueuedConnection,
                                  Q_ARG(QString, log));
    });
    if(!mProblemSolver->init())
    {
        mProblemSolver->deleteLater();
        QMetaObject::invokeMethod(mController.get(), "unlock", Qt::QueuedConnection);
        setMode(TSystemMode::Default);
        return;
    }

    auto questionRunner = [this]()
    {
       qDebug () << "CGravizSystem> RunSolver> questionRunner";
       this->setMode(TSystemMode::WaitForAnswer);
       mQuestioner.reset(new NCommand::CQuestioner(QStringList()
          << "-q" << "Save test to archive?"
          << "-o" << "y - Yes"
          << "-o" << "n - No"));
       connect(mQuestioner.get(), &NCommand::CQuestioner::log, [this](const QString& msg){
          QMetaObject::invokeMethod(mController.get(), "handleLog", Qt::QueuedConnection,
                                    Q_ARG(QString, msg));
       });
       connect(mQuestioner.get(), &NCommand::CQuestioner::finished, [this](int code){
          char ans = code;
          if(ans == 'y')
          {
             mController->handleLog("Saved\n");
             mTestProvider->addTest(NCommand::STest(mInputBuffer, mOutputBuffer));
          }
          mQuestioner->deleteLater();
//          for(int i = 0; i < mTestProvider->size(); ++i)
//             mController->handleLog(mTestProvider->getFormatted(i));
          mController->unlock();
          this->setMode(TSystemMode::Default);
       });
       mController->setQuestionMode();
       mQuestioner->run();
    };

    auto solverRunner = [this, questionRunner]()
    {
        QThread* solverThread = new QThread();
        mProblemSolver->moveToThread(solverThread);
        connect(solverThread, SIGNAL(started()), mProblemSolver, SLOT(run()));
        connect(solverThread, SIGNAL(finished()), solverThread, SLOT(deleteLater()));
        connect(solverThread, SIGNAL(finished()), mProblemSolver, SLOT(deleteLater()));
        connect(mProblemSolver, &NCommand::CProblemSolver::finished, [this, solverThread](int code)
        {
            qDebug () << "ProblemSolver thread was finished";
            QMetaObject::invokeMethod(mController.get(), "handleLog", Qt::QueuedConnection,
                                      Q_ARG(QString, QString(
                                            " [ Info ] Exit status: " + QString::number(code) +
                                          "\n [ Info ] Checker message: " + "No checker" + "\n")));
            if(!mProblemSolver->saveTestFlag())
            {
                QMetaObject::invokeMethod(mController.get(), "unlock", Qt::QueuedConnection);
                this->setMode(TSystemMode::Default);
            }
            solverThread->quit();
        });
        if(mProblemSolver->saveTestFlag())
        {
            connect(solverThread, &QThread::finished, [this, questionRunner](){
               qDebug () << "Input: " << mInputBuffer;
               qDebug () << "Output: " << mOutputBuffer;
                questionRunner();
            });
        }
        mInputBuffer.clear();
        mOutputBuffer.clear();
        solverThread->start();
    };

    QString sourceCodePath = NCommand::CFileSystem::getInstance().getFullPath(mProblemSolver->getSourceCodePath()).c_str();
    if(!mCompilerHandler->isSourceCode(sourceCodePath))
    {
        mCompilerHandler->addSourceCodePath(sourceCodePath);
    }

    if(mCompilerHandler->isNeededCompilation(sourceCodePath))
    {
        qDebug () << "neededCompilation";
        std::shared_ptr<QMetaObject::Connection> pconn(new QMetaObject::Connection);
        QMetaObject::Connection &conn = *pconn;
        conn = connect(mCompilerHandler.get(), &NCommand::CCompilerHandler::finished,
                       [this, pconn, &conn, solverRunner](int code){
            QMetaObject::invokeMethod(mController.get(), "handleLog", Qt::QueuedConnection,
                                      Q_ARG(QString, " [ Info ] Ready\n"));
            QMetaObject::invokeMethod(mController.get(), "setAppMode", Qt::QueuedConnection);
            solverRunner();

            disconnect(conn);
        });
        mCompilerHandler->performCompilation(mProblemSolver->getSourceCodePath(), mProblemSolver->getCompilationFlags());
    }
    else
    {
        mController->setAppMode();
        solverRunner();
    }
}

template <>
void CGravizSystem::handle<TGravizCommand::System>(const QStringList &args)
{
    qDebug () << "CGravizSystem> SystemCommand " << args;
    NCommand::CSystemCmd* cmd = new NCommand::CSystemCmd(QStringList() << *args.begin());
    cmd->setTime(60*60*3);
    connect(cmd, &NCommand::CSystemCmd::error, [this](const QString& msg){
        mController->handleError(msg);
    });
    connect(cmd, &NCommand::CSystemCmd::log, [this](const QString& msg){
        mController->handleLog(msg);
    });
    connect(cmd, &NCommand::CSystemCmd::finished, [this, cmd](int code){
//        mController->handleLog(" [ Info ] Finished with code " + QString::number(code) + "\n");
        mController->unlock();
        cmd->deleteLater();
    });
    cmd->setWorkingDir(NCommand::CFileSystem::getInstance().getCurrentPath());
    cmd->run();
}

template <>
void CGravizSystem::handle<TGravizCommand::TerminateProcess>(const QStringList &args)
{
    qDebug () << "CGravizSystem> terminate " << args;
    mProblemSolver->terminate();
}

template <>
void CGravizSystem::handle<TGravizCommand::Compile>(const QStringList &args)
{
    qDebug () << "CGravizSystem> Compile " << args;
    NCommand::CCompiler* compiler = new NCommand::CCompiler(args);
    connect(compiler, &NCommand::CCompiler::finished, [this, compiler](int code){
        compiler->deleteLater();
    });
    connect(compiler, &NCommand::CCompiler::error, [this](QString msg){
        mController->handleError(msg);
    });
    connect(compiler, &NCommand::CCompiler::log, [this](QString msg){
        mController->handleLog(msg);
    });
    connect(compiler, &NCommand::CCompiler::finished, [this](int code){
        mController->handleLog(" [ Info ] Finished with code " + QString::number(code) + "\n");
        mController->unlock();
    });
    compiler->run();
}

template <>
void CGravizSystem::handle<TGravizCommand::Test>(const QStringList &args)
{
    qDebug () << "CGravizSystem> Test " << args;
    QThread* testThread = new QThread();
    NCommand::CTestCommand* test = new NCommand::CTestCommand(args, mTestProvider);
    test->moveToThread(testThread);
    connect(test, &NCommand::CTestCommand::finished, [this, testThread, test](int code){
        testThread->quit();
        test->deleteLater();
        //mController->unlock();
        QMetaObject::invokeMethod(mController.get(), "unlock", Qt::QueuedConnection);
    });
    connect(testThread, SIGNAL(finished()), testThread, SLOT(deleteLater()));
    connect(test, &NCommand::CTestCommand::error, [this](QString msg){
//        mController->handleError(msg);
        QMetaObject::invokeMethod(mController.get(), "handleError", Qt::QueuedConnection,
                                  Q_ARG(QString, msg));
    });
    connect(test, &NCommand::CTestCommand::log, [this](QString msg){
//        mController->handleLog(msg);
        QMetaObject::invokeMethod(mController.get(), "handleLog", Qt::QueuedConnection,
                                  Q_ARG(QString, msg));
    });
    connect(testThread, SIGNAL(started()), test, SLOT(run()));
    testThread->start();
//    test->run();
}

template <>
void CGravizSystem::handle<TGravizCommand::Tester>(const QStringList &args)
{
    qDebug () << "CGravizSystem> Tester " << args;
    QThread* testerThread = new QThread();
    NCommand::CProblemTester* tester = new NCommand::CProblemTester(
                args,
                mTestProvider,
                mCompilerHandler);
    tester->moveToThread(testerThread);
    connect(tester, &NCommand::CProblemTester::finished, [this, testerThread, tester](int code){
        testerThread->quit();
        tester->deleteLater();
        //mController->unlock();
        QMetaObject::invokeMethod(mController.get(), "unlock", Qt::QueuedConnection);
    });
    connect(testerThread, SIGNAL(finished()), testerThread, SLOT(deleteLater()));
    connect(tester, &NCommand::CProblemTester::error, [this](QString msg){
//        mController->handleError(msg);
        QMetaObject::invokeMethod(mController.get(), "handleError", Qt::QueuedConnection,
                                  Q_ARG(QString, msg));
    });
    connect(tester, &NCommand::CProblemTester::log, [this](QString msg){
//        mController->handleLog(msg);
        QMetaObject::invokeMethod(mController.get(), "handleLog", Qt::QueuedConnection,
                                  Q_ARG(QString, msg));
    });
    connect(testerThread, SIGNAL(started()), tester, SLOT(run()));
    testerThread->start();
//    test->run();
}

template <>
void CGravizSystem::handle<TGravizCommand::ParseSite>(const QStringList &args)
{
    qDebug () << "CGravizSystem> ParseSite " << args;
    NCommand::CCodeforcesParser* parser = new NCommand::CCodeforcesParser(args);
    connect(parser, &NCommand::CCodeforcesParser::finished, [this, parser](int code){
        parser->deleteLater();
    });
    connect(parser, &NCommand::CCodeforcesParser::error, [this](QString msg){
        mController->handleError(msg);
    });
    connect(parser, &NCommand::CCodeforcesParser::log, [this](QString msg){
        mController->handleLog(msg);
    });
    connect(parser, &NCommand::CCodeforcesParser::finished, [this, parser](int code){
        //mController->handleLog(" [ Info ] Finished with code " + QString::number(code) + "\n");
        for(const NCommand::STest& test : parser->tests())
            mTestProvider->addTest(test);
        mController->unlock();
    });
    parser->run();
}

template <>
void CGravizSystem::handle<TGravizCommand::ChangeDirectory>(const QStringList &args)
{
    qDebug () << "CGravizSystem> ChangeDirectory " << args;
    NCommand::CFileSystem::getInstance().changeDir(*args.begin());
    mController->unlock();
}

template <>
void CGravizSystem::handle<TGravizCommand::Exit>(const QStringList &args)
{
    qDebug () << "CGravizSystem> Exit " << args;
    mController->exit();
}

template <>
void CGravizSystem::handle<TGravizCommand::Unknown>(const QStringList &args)
{
    qDebug () << "CGravizSystem> Unknown " << args;
    mController->handleError(QString(" [ Error ] ") + *args.begin() + " - No such command\n");
    mController->unlock();
}

template <>
void CGravizSystem::handle<TGravizCommand::Empty>(const QStringList &args)
{
    qDebug () << "CGravizSystem> Empty " << args;
    mController->unlock();
}

template <>
void CGravizSystem::handle<TGravizCommand::Total>(const QStringList &args)
{
    // nothing...
}

} // namespace NGraviz
