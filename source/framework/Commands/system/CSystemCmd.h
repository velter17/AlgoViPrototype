/**
 * Project   Graviz
 *
 * @file     CSystemCmd.h
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#pragma once

#include "framework/Commands/ITerminalCommand.h"

namespace NCommand
{

class CSystemCmd : public ITerminalCommand
{
public:
    CSystemCmd(QStringList args = QStringList());

protected:
    void run() override;
};

} // NCommand
