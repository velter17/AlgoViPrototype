/**
 * Project   Graviz
 *
 * @file     CCompiler.h
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#pragma once

#include "framework/Commands/ITerminalCommand.h"
#include "framework/Commands/system/CSystemCmd.h"

namespace NCommand
{

class CCompiler : public ITerminalCommand
{
public:
    CCompiler(QStringList args = QStringList());
    QString getManualMessage();

protected:
    void run() override;
};

} // namespace NCommand
