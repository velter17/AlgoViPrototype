/**
 * Project   Graviz
 *
 * @file     CTestCommand.h
 * @author   Dmytro Sadovyi
 * @date     09.12.2016
 */

#pragma once

#include <boost/shared_ptr.hpp>
#include "framework/Commands/ITerminalCommand.h"
#include "framework/Commands/ProblemSolver/CTestProvider.h"
#include "framework/Commands/ProblemSolver/CCompilerHandler.h"

namespace NCommand
{

class CTestCommand : public ITerminalCommand
{
public: // methods
    CTestCommand(const QStringList& args,
                 std::shared_ptr<CTestProvider> testProvider,
                 std::shared_ptr<CCompilerHandler> compilerHandler);

    void run() override;
    void appendData(const QString& data) override;
    void terminate() override;

private:
    void runGenerator(const QString& generatorPath,
                      const QString& solverPath,
                      int tests);
    void compile(const std::vector<QString> codes, size_t code_idx, int tests);

private: // fields
    std::shared_ptr<CTestProvider> mTestProvider;
    std::shared_ptr<CCompilerHandler> mCompilerHandler;
    int mCreateState;
    STest mCreatedTest;
};

} // namespace NCommand

