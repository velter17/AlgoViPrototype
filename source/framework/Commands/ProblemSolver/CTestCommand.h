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

namespace NCommand
{

class CTestCommand : public ITerminalCommand
{
public: // methods
    CTestCommand(const QStringList& args, std::shared_ptr<CTestProvider> testProvider);

    void run() override;
    void appendData(const QString& data) override;
    void terminate() override;

private: // fields
    std::shared_ptr<CTestProvider> mTestProvider;
};

} // namespace NCommand

