/**
 * Project   Graviz
 *
 * @file     CProblemTester.h
 * @author   Dmytro Sadovyi
 * @date     10.12.2016
 */

#pragma once

#include "framework/Commands/ITerminalCommand.h"
#include "framework/Commands/ProblemSolver/CTestProvider.h"
#include "framework/Commands/ProblemSolver/CCompilerHandler.h"

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

public slots: // methods
    void testRunner(int test);
    QString checkResult(int test);
private:
    std::shared_ptr<CTestProvider> mTestProvider;
    std::shared_ptr<CCompilerHandler> mCompilerHandler;
    QString mOutputBuffer;
    QString mSourceCodePath;
    int mTestFrom;
    int mTestTo;
    bool mNeedDetails;
};

} // namespace NCommand

