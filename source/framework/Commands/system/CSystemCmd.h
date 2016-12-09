/**
 * Project   Graviz
 *
 * @file     CSystemCmd.h
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#pragma once

#include <memory>
#include "framework/Commands/ITerminalCommand.h"

namespace NCommand
{

class CSystemCmd : public ITerminalCommand
{
public:
    CSystemCmd(QStringList args = QStringList());

public slots:
    void run() override;
    void terminate() override;
private:
    QProcess* mProc;
};

} // NCommand
