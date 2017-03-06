/**
 * Project   AlgoVi
 *
 * @file     CProblemTesterInteractive.h
 * @author   Dmytro Sadovyi
 * @date     01.03.2017
 */

#pragma once

#include "framework/Commands/ITerminalCommand.h"
#include "framework/Commands/ProblemSolver/CTestProvider.h"
#include "framework/Commands/ProblemSolver/CCompilerHandler.h"
#include "framework/Commands/ProblemSolver/CProblemSolver.h"

namespace NCommand
{

class CProblemTesterInteractive : public ITerminalCommand
{
public: // methods
    CProblemTesterInteractive(const QStringList& args,
                   std::shared_ptr<CTestProvider> testProvider,
                   std::shared_ptr<CCompilerHandler> compilerHandler);

    void run() override;
    void appendData(const QString& data) override;
    void terminate() override;

    QString getManualMessage() override;

public slots: // methods
    void compile(const std::vector<QString> codes, size_t code_idx);
    void runTester(int test);
    QString checkResult(int test, int returnCode);
private:
    std::shared_ptr<CTestProvider> mTestProvider;
    std::shared_ptr<CCompilerHandler> mCompilerHandler;
    QString mOutputBuffer;
    QString mErrorBuffer;
    QString mSourceCodePath;
    QString mCheckerCodePath;
    QString mInteractorCodePath;
    bool mTerminateFlag;
    QString mCheckerType;
    bool mCustomChecker;
    CProblemSolver* mProblemSolverPtr;
    CProblemSolver* mInteractorPtr;
    int mTestFrom;
    int mTestTo;
    bool mDetailedReport;
    bool mFullProtocol;
    int mAcceptedCounter;
    QTemporaryDir mDir;
    std::vector <std::pair<bool, QString> > mInteractionProtocol;
};

} // namespace NCommand

