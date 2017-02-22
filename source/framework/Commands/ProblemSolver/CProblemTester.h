/**
 * Project   AlgoVi
 *
 * @file     CProblemTester.h
 * @author   Dmytro Sadovyi
 * @date     10.12.2016
 */

#pragma once

#include "framework/Commands/ITerminalCommand.h"
#include "framework/Commands/ProblemSolver/CTestProvider.h"
#include "framework/Commands/ProblemSolver/CCompilerHandler.h"
#include "framework/Commands/ProblemSolver/CProblemSolver.h"

namespace NCommand
{

class CProblemTester : public ITerminalCommand
{
public: // methods
    CProblemTester(const QStringList& args,
                   std::shared_ptr<CTestProvider> testProvider,
                   std::shared_ptr<CCompilerHandler> compilerHandler);

    void run() override;
    void appendData(const QString& data) override;
    void terminate() override;

    QString getManualMessage() override;

public slots: // methods
    void checkerCompiler();
    void testRunner(int test);
    QString checkResult(int test, int returnCode);
private:
    std::shared_ptr<CTestProvider> mTestProvider;
    std::shared_ptr<CCompilerHandler> mCompilerHandler;
    QString mOutputBuffer;
    QString mErrorBuffer;
    QString mSourceCodePath;
    QString mCheckerType;
    bool mCustomChecker;
    int mTestFrom;
    int mTestTo;
    bool mTerminateFlag;
    int mAcceptedCounter;
    CProblemSolver* mProblemSolverPtr;
    bool mNeedDetails;
};

} // namespace NCommand

