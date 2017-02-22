/**
 * Project   AlgoVi
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

enum class TestCommandState
{
    CreateTest,
    EditTest,
    None,
};

class CTestCommand : public ITerminalCommand
{
    Q_OBJECT
public: // methods
    CTestCommand(const QStringList& args,
                 std::shared_ptr<CTestProvider> testProvider,
                 std::shared_ptr<CCompilerHandler> compilerHandler);

    void run() override;
    void appendData(const QString& data) override;
    void terminate() override;

signals:
    void showInWindow(const QString& input, const QString& output);
    void startEditMode(const QString& input, const QString& output);

private:
    std::pair<int, int> parseTestRange(const QString& str);

    void runGenerator(const QString& generatorPath,
                      const QString& solverPath,
                      int tests);
    void runValidator(const QString& validatorPath);
    void compile(const std::vector<QString> codes, size_t code_idx, int tests);

private: // fields
    std::shared_ptr<CTestProvider> mTestProvider;
    std::shared_ptr<CCompilerHandler> mCompilerHandler;
    int mCreateState;
    STest mCreatedTest;
    TestCommandState mState;
    bool mConsoleMode;
    int mTestToEdit;
    QString mValidatorResultMessage;
    int mValidationCounter;
};

} // namespace NCommand

