/**
 * Project   AlgoVi
 *
 * @file     CAlgoViSystem.h
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#pragma once

#include <QObject>
#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QDateTime>

#include "Controller/CSystemController.h"
#include "algovi/CCommandHandler.h"
#include "algovi/Types.h"
#include "framework/Commands/ProblemSolver/CProblemSolver.h"
#include "framework/Commands/ProblemSolver/CCompilerHandler.h"
#include "framework/Commands/ProblemSolver/CTestProvider.h"
#include "framework/Commands/ProblemSolver/CTestCommand.h"
#include "framework/Commands/common/CQuestioner.h"

namespace NController {
class CSystemController;
}

namespace NAlgoVi
{

class CCommandHandler;

class CAlgoViSystem : public QObject
{
    Q_OBJECT
public: // methods
    CAlgoViSystem(std::shared_ptr<NController::CSystemController> controll);

    void handleCommand(NController::TTerminalCommandType type, const QString& cmd);

    template <TAlgoViCommand command>
    void handle(const QStringList& args);

private: // methods
    template <TAlgoViCommand command>
    void handleFuncRegistrator();

public slots: // methods
    void setMode(TSystemMode mode);

private: // fields
    std::shared_ptr<NController::CSystemController> mController;
    std::shared_ptr<NView::CGraphicView> mView;
    std::shared_ptr<CCommandHandler> mCommandHandler;
    std::shared_ptr<NCommand::CCompilerHandler> mCompilerHandler;
    NCommand::CQuestioner* mQuestioner;
    std::shared_ptr<NCommand::CTestProvider> mTestProvider;
    std::shared_ptr<NCommand::CTestCommand> mTestCommand;
    NCommand::CProblemSolver* mProblemSolver;
    NCommand::ITerminalCommand* mTerminalCommand;

    TSystemMode mMode;
    QString mInputBuffer;
    QString mOutputBuffer;

    typedef void (CAlgoViSystem::*THandler)(const QString& args);

    THandler mCommandHandlers[(size_t)TAlgoViCommand::Total];
};

} // namespace NAlgoVi
