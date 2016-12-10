/**
 * Project   Graviz
 *
 * @file     IProblemChecker.h
 * @author   Dmytro Sadovyi
 * @date     10.12.2016
 */

#pragma once

#include "framework/Commands/ITerminalCommand.h"

namespace NCommand
{

enum class TCheckerResult
{
    Success,
    Fail,
    None,
};

class IProblemChecker : public ITerminalCommand
{
public: // methods
    IProblemChecker(const QStringList& args);

    void run() override;
    void appendData(const QString &data) override;
    void terminate() override;

    virtual TCheckerResult check() = 0;

    QString details();
    TCheckerResult getResult();

protected: // fields
    QString mCheckerMessage;
    TCheckerResult mResultCode;
    boost::program_options::variables_map mVarMap;
};

} // namespace NCommand

