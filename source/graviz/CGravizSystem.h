/**
 * Project   Graviz
 *
 * @file     CGravizSystem.h
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#pragma once

#include <QObject>
#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QDateTime>

#include "Controller/CSystemController.h"
#include "graviz/CCommandHandler.h"
#include "graviz/Types.h"
#include "framework/Commands/ProblemSolver/CProblemSolver.h"
#include "framework/Commands/ProblemSolver/CCompilerHandler.h"
#include "framework/Commands/ProblemSolver/CTestProvider.h"
#include "framework/Commands/ProblemSolver/CTestCommand.h"
#include "framework/Commands/common/CQuestioner.h"

namespace NController {
class CSystemController;
}

namespace NGraviz
{

class CCommandHandler;

class CGravizSystem : public QObject
{
    Q_OBJECT
public: // methods
    CGravizSystem(std::shared_ptr<NController::CSystemController> controll);

    void handleCommand(NController::TTerminalCommandType type, const QString& cmd);

    template <TGravizCommand command>
    void handle(const QStringList& args);

private: // methods
    template <TGravizCommand command>
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

    typedef void (CGravizSystem::*THandler)(const QString& args);

    THandler mCommandHandlers[(size_t)TGravizCommand::Total];
};

} // namespace NGraviz
