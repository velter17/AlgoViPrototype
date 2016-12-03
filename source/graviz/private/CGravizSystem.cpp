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

#include "graviz/CGravizSystem.h"
#include "framework/Commands/system/CCompiler.h"

namespace NGraviz
{

CGravizSystem::CGravizSystem(std::shared_ptr<NController::CSystemController> controll)
   : mView(new NView::CGraphicView())
   , mController(controll)
   , mMode(TSystemMode::Interaction)
   , mCommandHandler(new CCommandHandler(this))
   , mProblemSolver(new CProblemSolver())
   , mProblemCompiler(new CProblemCompiler())
{
    mController->setModel(std::shared_ptr<CGravizSystem>(this));
    mController->setView(mView);
    connect(mCommandHandler.get(), &CCommandHandler::endCommand,
            [this]{mController->unlock();});
    connect(mCommandHandler.get(), &CCommandHandler::log,
            [this](QString msg){mController->handleLog(msg);});
    connect(mCommandHandler.get(), &CCommandHandler::error,
            [this](QString msg){mController->handleError(msg);});

    QThread *thread = new QThread();
    mProblemSolver->moveToThread(thread);

    connect(mProblemSolver.get(), SIGNAL(out(QString)),
            mController.get(), SLOT(handleLog(QString)));
    connect(mProblemSolver.get(), SIGNAL(err(QString)),
            mController.get(), SLOT(handleError(QString)));
    connect(mProblemSolver.get(), &CProblemSolver::finished,
            [this](int code, QString checkerMsg)
    {
        qDebug () << "ProblemSolver thread was finished";
        QMetaObject::invokeMethod(mController.get(), "handleLog", Qt::QueuedConnection,
                                  Q_ARG(QString, QString("-----------------------------\n") +
                                        "Exit status: " + QString::number(code) +
                                        "\nChecker message: " + checkerMsg +
                                        "\n-----------------------------"));
        QMetaObject::invokeMethod(mController.get(), "unlock", Qt::QueuedConnection);
        this->setMode(TSystemMode::Interaction);
    });

    thread->start();


    mSourceCodeFileInfo.setFile("/home/dsadovyi/Coding/code.cpp");
    mLastModified = QDateTime();
}

void CGravizSystem::handleCommand(NController::TTerminalCommandType type, const QString &cmd)
{
    qDebug () << "CGravizSystem> handleCommand " << cmd;
    if(mMode == TSystemMode::Execution)
    {
        assert(0 != mProblemSolver);
        qDebug () << "append data " << cmd;
        if(cmd == "^C")
        {
            QMetaObject::invokeMethod(mProblemSolver.get(),"terminate", Qt::QueuedConnection);
        }
        else
        {
            QMetaObject::invokeMethod(mProblemSolver.get(),"appendData", Qt::QueuedConnection,
                                      Q_ARG(QString, cmd));
        }
    }
    else
    {
        mCommandHandler->handle(cmd);
    }
}

void CGravizSystem::runSolver(QString inputData)
{
    mProblemSolver->setSettings(CSolverSettings()
                        .setSolverAppPath("/home/dsadovyi/Coding/app")
                        .setTimeLimit(10000000)
                        .setType(TProblemSolverType::Interactive));
    setMode(TSystemMode::Execution);

    mSourceCodeFileInfo.refresh();
    if(mLastModified.toTime_t() != mSourceCodeFileInfo.lastModified().toTime_t() && false)
    {
        mController->handleLog("+-----------------------------------+\n"
                               "|......Source code was modified.....|\n"
                               "|......Compiling application....... |\n"
                               "+-----------------------------------+\n";
        mLastModified = mSourceCodeFileInfo.lastModified();

        connect(mCommandHandler.get(), &CCommandHandler::endSystemCommand, [this, inputData](){
            qDebug () << "Compilation was finished....";
            mController->setAppMode();
            QMetaObject::invokeMethod(mProblemSolver.get(), "solve", Qt::QueuedConnection,
                                      Q_ARG(QString, inputData));
        });
        compileSourceCode(mSourceCodeFileInfo.absoluteFilePath());
    }
    else
    {
        mController->setAppMode();
        QMetaObject::invokeMethod(mProblemSolver.get(), "solve", Qt::QueuedConnection,
                                  Q_ARG(QString, inputData));
    }
}

void CGravizSystem::setMode(TSystemMode mode)
{
    mMode = mode;
}

void CGravizSystem::compileSourceCode(const QString &path)
{
    qDebug () << path << " was changed";
    NCommand::CFileSystem::getInstance().remove("/home/dsadovyi/Coding/app");
    mCommandHandler->handle("compile -s " + path, true);
}

} // namespace NGraviz
