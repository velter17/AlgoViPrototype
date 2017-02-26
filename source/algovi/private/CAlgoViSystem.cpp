/**
 * Project   AlgoVi
 *
 * @file     CAlgoViSystem.cpp
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#include <QProcess>
#include <QDebug>
#include <QThread>
#include <memory>

#include "algovi/CAlgoViSystem.h"
#include "framework/Commands/system/CCompiler.h"
#include "framework/Commands/ProblemSolver/CTestCommand.h"
#include "framework/Commands/ProblemSolver/ProblemParser/CCodeforcesParser.h"
#include "framework/Commands/ProblemSolver/CProblemTester.h"
#include "framework/Commands/ProblemSolver/CProblemTesterInteractive.h"
#include "framework/Commands/ProblemSolver/CVisualSolver.h"
#include "View/CVisualizationController.h"


namespace NAlgoVi
{

CAlgoViSystem::CAlgoViSystem(std::shared_ptr<NController::CSystemController> controll)
   : mView(new NView::CGraphicView())
   , mController(controll)
   , mMode(TSystemMode::Default)
   , mCommandHandler(new CCommandHandler(this))
   , mCompilerHandler(new NCommand::CCompilerHandler())
   , mProblemSolver(nullptr)
   , mTestProvider(new NCommand::CTestProvider())
{
    mController->setModel(std::shared_ptr<CAlgoViSystem>(this));
    mController->setView(mView);

    connect(&NCommand::CFileSystem::getInstance(), &NCommand::CFileSystem::error, [this](const QString &msg){
        mController->handleError(msg);
    });

//    QThread* compilerHandlerThread = new QThread();
//    mCompilerHandler->moveToThread(compilerHandlerThread);
    connect(mCompilerHandler.get(), &NCommand::CCompilerHandler::out, [this](const QString& msg){
        QMetaObject::invokeMethod(mController.get(), "handleLog", Qt::QueuedConnection, Q_ARG(QString, msg));
    });
    connect(mCompilerHandler.get(), &NCommand::CCompilerHandler::err, [this](const QString& msg){
        QMetaObject::invokeMethod(mController.get(), "handleError", Qt::QueuedConnection, Q_ARG(QString, msg));
    });
//    connect(mCompilerHandler.get(), SIGNAL(destroyed(QObject*)), compilerHandlerThread, SLOT(deleteLater()));
//    compilerHandlerThread->start();
}

void CAlgoViSystem::handleCommand(NController::TTerminalCommandType type, const QString &cmd)
{
    qDebug () << "CAlgoViSystem> handleCommand " << cmd;
//    if(mMode == TSystemMode::InProcess)
    if(mMode != TSystemMode::Default)
    {
        qDebug () << "CAlgoViSystem> terminate";
        if(cmd == "^C")
        {
            QMetaObject::invokeMethod(mTerminalCommand,"terminate", Qt::QueuedConnection);
        }
        else
        {
            qDebug () << "append data " << cmd;
            if(mMode == TSystemMode::InProcess)
            {
                mInputBuffer += cmd + "\n";
            }
            QMetaObject::invokeMethod(mTerminalCommand, "appendData", Qt::QueuedConnection,
                                      Q_ARG(QString, cmd));
        }
    }
//    else if(mMode == TSystemMode::WaitForAnswer)
//    {
//        mQuestioner->appendData(cmd);
//    }
    else
    {
        mCommandHandler->handle(cmd);
    }
}


void CAlgoViSystem::setMode(TSystemMode mode)
{
    mMode = mode;
}


template <TAlgoViCommand command>
void CAlgoViSystem::handleFuncRegistrator()
{
    mCommandHandlers[(size_t)command] = &CAlgoViSystem::handle<command>;
    handleFuncRegistrator<(TAlgoViCommand)((size_t)command+1)>();
}

template <>
void CAlgoViSystem::handleFuncRegistrator<TAlgoViCommand::Total>()
{

}

template <>
void CAlgoViSystem::handle<TAlgoViCommand::RunSolver>(const QStringList &args)
{
    qDebug () << "CAlgoViSystem> RunSolver " << args;
    setMode(TSystemMode::InProcess);
    //mProblemSolver.reset(new NCommand::CProblemSolver(args));
    mProblemSolver = new NCommand::CProblemSolver(args, mTestProvider);
    mTerminalCommand = mProblemSolver;
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
    connect(mProblemSolver, &NCommand::CProblemSolver::logHtml, [this](const QString& log){
        //mController->handleError(log);
        QMetaObject::invokeMethod(mController.get(), "handleLogHtml", Qt::QueuedConnection,
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
       qDebug () << "CAlgoViSystem> RunSolver> questionRunner";
       this->setMode(TSystemMode::WaitForAnswer);
       mQuestioner = new NCommand::CQuestioner(QStringList()
          << "-q" << "Save test to archive?"
          << "-o" << "y - Yes"
          << "-o" << "n - No");
       mTerminalCommand = mQuestioner;
       QThread* questionerThread = new QThread();
       mQuestioner->moveToThread(questionerThread);
       connect(mQuestioner, &NCommand::CQuestioner::log, [this](const QString& msg){
          QMetaObject::invokeMethod(mController.get(), "handleLog", Qt::QueuedConnection,
                                    Q_ARG(QString, msg));
       });
       connect(questionerThread, SIGNAL(started()), mQuestioner, SLOT(run()));
       connect(mQuestioner, &NCommand::CQuestioner::finished, [this, questionerThread](int code){
          char ans = code;
          if(ans == 'y')
          {
             //mController->handleLog("Saved\n");
              QMetaObject::invokeMethod(mController.get(), "handleLog", Qt::QueuedConnection,
                                        Q_ARG(QString, "Saved\n"));
             mTestProvider->addTest(NCommand::STest(mInputBuffer, mOutputBuffer));
          }
          //mQuestioner->deleteLater();
          questionerThread->quit();
//          for(int i = 0; i < mTestProvider->size(); ++i)
//             mController->handleLog(mTestProvider->getFormatted(i));
          //mController->unlock();
          QMetaObject::invokeMethod(mController.get(), "unlock", Qt::QueuedConnection);
          this->setMode(TSystemMode::Default);
       });
       connect(questionerThread, SIGNAL(finished()), mQuestioner, SLOT(deleteLater()));
       connect(questionerThread, SIGNAL(finished()), questionerThread, SLOT(deleteLater()));
       mController->setQuestionMode();
       questionerThread->start();
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
                                            " [ Info ] Exit status: " + QString::number(code) + "\n")));
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

    QString sourceCodePath = NCommand::CFileSystem::getInstance().getFullPath(mProblemSolver->getSourceCodePath()).string().c_str();
    if(!mCompilerHandler->isSourceCode(sourceCodePath))
    {
        mCompilerHandler->addSourceCodePath(sourceCodePath);
    }
    mProblemSolver->setAppPath(mCompilerHandler->getExecCmd(sourceCodePath));

    if(mCompilerHandler->isNeededCompilation(sourceCodePath))
    {
        qDebug () << "neededCompilation";
        std::shared_ptr<QMetaObject::Connection> pconn(new QMetaObject::Connection);
        QMetaObject::Connection &conn = *pconn;
        conn = connect(mCompilerHandler.get(), &NCommand::CCompilerHandler::finished,
                       [this, pconn, &conn, solverRunner](int code){
            if(code == 0)
            {
                QMetaObject::invokeMethod(mController.get(), "handleLog", Qt::QueuedConnection,
                                          Q_ARG(QString, " [ Info ] Ready\n"));
                QMetaObject::invokeMethod(mController.get(), "setAppMode", Qt::QueuedConnection);
                solverRunner();
            }
            else
            {
                QMetaObject::invokeMethod(mController.get(), "unlock", Qt::QueuedConnection);
                this->setMode(TSystemMode::Default);
                mProblemSolver->deleteLater();
            }

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
void CAlgoViSystem::handle<TAlgoViCommand::RunSolverVisual>(const QStringList &args)
{
    qDebug () << "CAlgoViSystem> RunSolverVisual " << args;
    NCommand::CVisualSolver* solver = new NCommand::CVisualSolver(args, mCompilerHandler);
    mController->setAppMode();
    setMode(TSystemMode::InProcess);
    mTerminalCommand = solver;
    connect(solver, &NCommand::CVisualSolver::error, [this](const QString& msg){
//        mController->handleError(msg);
        QMetaObject::invokeMethod(mController.get(), "handleError", Qt::QueuedConnection,
                                  Q_ARG(QString, msg));
    });
    connect(solver, &NCommand::CVisualSolver::logHtml, [this](QString msg){
//        mController->handleLogHtml(msg);
        QMetaObject::invokeMethod(mController.get(), "handleHtml", Qt::QueuedConnection,
                                  Q_ARG(QString, msg));
    });
    if(!solver->init())
    {
        solver->deleteLater();
        QMetaObject::invokeMethod(mController.get(), "unlock", Qt::QueuedConnection);
        setMode(TSystemMode::Default);
        return;
    }

    NView::CVisualizationController* controller = new NView::CVisualizationController(
                mView,
                mCompilerHandler,
                solver);
    connect(controller, &NView::CVisualizationController::sceneChanged, [this, solver](const QString& data){
        QMetaObject::invokeMethod(solver, "setInput", Qt::QueuedConnection, Q_ARG(QString, data));
    });

    std::shared_ptr<QMetaObject::Connection> pconn(new QMetaObject::Connection);
    QMetaObject::Connection &conn = *pconn;
    conn = connect(solver, SIGNAL(log(QString)), controller, SLOT(updateResult(QString)));
    connect(solver, SIGNAL(command(QString)), controller, SLOT(handleInput(QString)));

    QThread* solverThread = new QThread();
    solver->moveToThread(solverThread);
    connect(solverThread, SIGNAL(started()), solver, SLOT(run()));
    connect(solverThread, SIGNAL(finished()), solverThread, SLOT(deleteLater()));
    connect(solverThread, SIGNAL(finished()), solver, SLOT(deleteLater()));
    connect(solver, &NCommand::CVisualSolver::finished, [this, solver, solverThread, controller, pconn, &conn](int code){
//        mController->handleLog(" [ Info ] Finished with code " + QString::number(code) + "\n");
//        mController->unlock();
        QMetaObject::invokeMethod(mController.get(), "unlock", Qt::QueuedConnection);
        solverThread->quit();
        disconnect(conn);
        controller->finish();
        controller->deleteLater();
        setMode(TSystemMode::Default);
    });

    solverThread->start();

}

template <>
void CAlgoViSystem::handle<TAlgoViCommand::System>(const QStringList &args)
{
    qDebug () << "CAlgoViSystem> SystemCommand " << args;
    NCommand::CSystemCmd* cmd = new NCommand::CSystemCmd(args);
    cmd->setTime(60*60*3);
    mController->setAppMode();
    connect(cmd, &NCommand::CSystemCmd::error, [this](const QString& msg){
        mController->handleError(msg);
    });
    connect(cmd, &NCommand::CSystemCmd::log, [this](const QString& msg){
        mController->handleLog(msg);
    });
    connect(cmd, &NCommand::CSystemCmd::logHtml, [this](QString msg){
        mController->handleLogHtml(msg);
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
void CAlgoViSystem::handle<TAlgoViCommand::Python>(const QStringList &args)
{
    qDebug () << "CAlgoViSystem> Python " << args;
    setMode(TSystemMode::InProcess);
    mProblemSolver = new NCommand::CProblemSolver(QStringList() << args << "-s" << "Vika<3", mTestProvider);
    mTerminalCommand = mProblemSolver;
    connect(mProblemSolver, &NCommand::CProblemSolver::log, [this](const QString& log){
        //mController->handleLog(log);
        QMetaObject::invokeMethod(mController.get(), "handleLog", Qt::QueuedConnection,
                                  Q_ARG(QString, log));
    });
    connect(mProblemSolver, &NCommand::CProblemSolver::error, [this](const QString& log){
        QMetaObject::invokeMethod(mController.get(), "handleError", Qt::QueuedConnection,
                                  Q_ARG(QString, log));
    });
    connect(mProblemSolver, &NCommand::CProblemSolver::logHtml, [this](const QString& log){
        QMetaObject::invokeMethod(mController.get(), "handleLogHtml", Qt::QueuedConnection,
                                  Q_ARG(QString, log));
    });
    if(!mProblemSolver->init())
    {
        mProblemSolver->deleteLater();
        QMetaObject::invokeMethod(mController.get(), "unlock", Qt::QueuedConnection);
        setMode(TSystemMode::Default);
        return;
    }

#ifdef WIN_TARGET
    mProblemSolver->setAppPath("D:\\Programs\\Python\\python.exe");
#else
    mProblemSolver->setAppPath("/usr/bin/python -i");
#endif

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
                                        " [ Info ] Exit status: " + QString::number(code) + "\n")));
        QMetaObject::invokeMethod(mController.get(), "unlock", Qt::QueuedConnection);
        this->setMode(TSystemMode::Default);
        solverThread->quit();
    });
    mController->setAppMode();
    solverThread->start();
}

template <>
void CAlgoViSystem::handle<TAlgoViCommand::TerminateProcess>(const QStringList &args)
{
    qDebug () << "CAlgoViSystem> terminate " << args;
    mProblemSolver->terminate();
}

template <>
void CAlgoViSystem::handle<TAlgoViCommand::Compile>(const QStringList &args)
{
    qDebug () << "CAlgoViSystem> Compile " << args;
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
        setMode(TSystemMode::Default);
    });
    connect(compiler, &NCommand::CCompiler::logHtml, [this](QString msg){
        mController->handleLogHtml(msg);
    });
    setMode(TSystemMode::InProcess);
    mTerminalCommand = compiler;
    compiler->run();
}

template <>
void CAlgoViSystem::handle<TAlgoViCommand::Test>(const QStringList &args)
{
    qDebug () << "CAlgoViSystem> Test " << args;
    QThread* testThread = new QThread();
    NCommand::CTestCommand* test = new NCommand::CTestCommand(args, mTestProvider, mCompilerHandler);
    mTerminalCommand = test;
    test->moveToThread(testThread);
    connect(test, &NCommand::CTestCommand::finished, [this, testThread, test](int code){
        testThread->quit();
        test->deleteLater();
        //mController->unlock();
        setMode(TSystemMode::Default);
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
    connect(test, &NCommand::CTestCommand::logHtml, [this](QString msg){
        QMetaObject::invokeMethod(mController.get(), "handleLogHtml", Qt::QueuedConnection,
                                  Q_ARG(QString, msg));
    });
    connect(test, &NCommand::CTestCommand::showInWindow, [this](const QString& input, const QString& output){
        std::shared_ptr<QMetaObject::Connection> pconn1(new QMetaObject::Connection);
        QMetaObject::Connection &conn1 = *pconn1;
        conn1 = connect(mController.get(), &NController::CSystemController::ioOkButtonPressed,
                       [this, pconn1, &conn1](){
            QMetaObject::invokeMethod(mController.get(), "showIoWin", Qt::QueuedConnection, Q_ARG(bool, false));
            disconnect(conn1);
        });

        std::shared_ptr<QMetaObject::Connection> pconn2(new QMetaObject::Connection);
        QMetaObject::Connection &conn2 = *pconn2;
        conn2 = connect(mController.get(), &NController::CSystemController::ioCancelButtonPressed,
                       [this, pconn2, &conn2](){
            QMetaObject::invokeMethod(mController.get(), "showIoWin", Qt::QueuedConnection, Q_ARG(bool, false));
            disconnect(conn2);
        });

        QMetaObject::invokeMethod(mController.get(), "showIoWin", Qt::QueuedConnection, Q_ARG(bool, true));
        QMetaObject::invokeMethod(mController.get(), "setIoData", Qt::QueuedConnection,
                                  Q_ARG(QString, input),
                                  Q_ARG(QString, output));
    });

    connect(test, &NCommand::CTestCommand::startEditMode, [this, test](const QString& input, const QString& output){
        std::shared_ptr<QMetaObject::Connection> pconn1(new QMetaObject::Connection);
        QMetaObject::Connection &conn1 = *pconn1;
        conn1 = connect(mController.get(), &NController::CSystemController::ioOkButtonPressed,
                       [this, pconn1, &conn1, test](const QString& input, const QString& output){
            QMetaObject::invokeMethod(test, "appendData", Qt::QueuedConnection, Q_ARG(QString, input));
            QMetaObject::invokeMethod(test, "appendData", Qt::QueuedConnection, Q_ARG(QString, output));
            QMetaObject::invokeMethod(mController.get(), "showIoWin", Qt::QueuedConnection, Q_ARG(bool, false));
            disconnect(conn1);
        });

        std::shared_ptr<QMetaObject::Connection> pconn2(new QMetaObject::Connection);
        QMetaObject::Connection &conn2 = *pconn2;
        conn2 = connect(mController.get(), &NController::CSystemController::ioCancelButtonPressed,
                       [this, pconn2, &conn2, test](){
            QMetaObject::invokeMethod(mController.get(), "showIoWin", Qt::QueuedConnection, Q_ARG(bool, false));
            QMetaObject::invokeMethod(test, "terminate", Qt::QueuedConnection);
            disconnect(conn2);
        });

        QMetaObject::invokeMethod(mController.get(), "showIoWin", Qt::QueuedConnection, Q_ARG(bool, true));
        QMetaObject::invokeMethod(mController.get(), "setIoData", Qt::QueuedConnection,
                                  Q_ARG(QString, input),
                                  Q_ARG(QString, output));

        setMode(TSystemMode::Default);
    });


    connect(testThread, SIGNAL(started()), test, SLOT(run()));
    setMode(TSystemMode::InProcess);
    mController->setAppMode();
    testThread->start();
//    test->run();
}

template <>
void CAlgoViSystem::handle<TAlgoViCommand::Tester>(const QStringList &args)
{
    qDebug () << "CAlgoViSystem> Tester " << args;
    QThread* testerThread = new QThread();
    NCommand::CProblemTester* tester = new NCommand::CProblemTester(
                args,
                mTestProvider,
                mCompilerHandler);
    mTerminalCommand = tester;
    tester->moveToThread(testerThread);
    connect(tester, &NCommand::CProblemTester::finished, [this, testerThread, tester](int code){
        testerThread->quit();
        tester->deleteLater();
        //mController->unlock();
        setMode(TSystemMode::Default);
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
    connect(tester, &NCommand::CProblemTester::logHtml, [this](QString msg){
//        mController->handleLog(msg);
        QMetaObject::invokeMethod(mController.get(), "handleLogHtml", Qt::QueuedConnection,
                                  Q_ARG(QString, msg));
    });
    connect(testerThread, SIGNAL(started()), tester, SLOT(run()));
    setMode(TSystemMode::InProcess);
    QMetaObject::invokeMethod(mController.get(), "setAppMode", Qt::QueuedConnection);
    testerThread->start();
//    test->run();
}

template <>
void CAlgoViSystem::handle<TAlgoViCommand::TesterInteractive>(const QStringList &args)
{
    qDebug () << "CAlgoViSystem> TesterInteractive " << args;
    QThread* testerThread = new QThread();
    NCommand::CProblemTesterInteractive* tester = new NCommand::CProblemTesterInteractive(
                args,
                mTestProvider,
                mCompilerHandler);
    mTerminalCommand = tester;
    tester->moveToThread(testerThread);
    connect(tester, &NCommand::CProblemTesterInteractive::finished, [this, testerThread, tester](int code){
        testerThread->quit();
        tester->deleteLater();
        //mController->unlock();
        setMode(TSystemMode::Default);
        QMetaObject::invokeMethod(mController.get(), "unlock", Qt::QueuedConnection);
    });
    connect(testerThread, SIGNAL(finished()), testerThread, SLOT(deleteLater()));
    connect(tester, &NCommand::CProblemTesterInteractive::error, [this](QString msg){
//        mController->handleError(msg);
        QMetaObject::invokeMethod(mController.get(), "handleError", Qt::QueuedConnection,
                                  Q_ARG(QString, msg));
    });
    connect(tester, &NCommand::CProblemTesterInteractive::log, [this](QString msg){
//        mController->handleLog(msg);
        QMetaObject::invokeMethod(mController.get(), "handleLog", Qt::QueuedConnection,
                                  Q_ARG(QString, msg));
    });
    connect(tester, &NCommand::CProblemTesterInteractive::logHtml, [this](QString msg){
//        mController->handleLog(msg);
        QMetaObject::invokeMethod(mController.get(), "handleLogHtml", Qt::QueuedConnection,
                                  Q_ARG(QString, msg));
    });
    connect(testerThread, SIGNAL(started()), tester, SLOT(run()));
    setMode(TSystemMode::InProcess);
    QMetaObject::invokeMethod(mController.get(), "setAppMode", Qt::QueuedConnection);
    testerThread->start();
//    test->run();
}

template <>
void CAlgoViSystem::handle<TAlgoViCommand::ParseSite>(const QStringList &args)
{
    qDebug () << "CAlgoViSystem> ParseSite " << args;
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
        mController->handleLog(" [ Info ] " + QString::number(parser->tests().size()) + " tests were parsed\n");
        mController->unlock();
    });
    parser->run();
}

template <>
void CAlgoViSystem::handle<TAlgoViCommand::ChangeDirectory>(const QStringList &args)
{
    qDebug () << "CAlgoViSystem> ChangeDirectory " << args;
    NCommand::CFileSystem::getInstance().changeDir(*args.begin());
    mController->unlock();
}

template <>
void CAlgoViSystem::handle<TAlgoViCommand::Exit>(const QStringList &args)
{
    qDebug () << "CAlgoViSystem> Exit " << args;
    mController->exit();
}

template <>
void CAlgoViSystem::handle<TAlgoViCommand::Unknown>(const QStringList &args)
{
    qDebug () << "CAlgoViSystem> Unknown " << args;
    mController->handleError(QString(" [ Error ] ") + *args.begin() + " - No such command\n");
    mController->unlock();
}

template <>
void CAlgoViSystem::handle<TAlgoViCommand::Empty>(const QStringList &args)
{
    qDebug () << "CAlgoViSystem> Empty " << args;
    mController->unlock();
}

template <>
void CAlgoViSystem::handle<TAlgoViCommand::Total>(const QStringList &args)
{
    // nothing...
}

} // namespace NAlgoVi
