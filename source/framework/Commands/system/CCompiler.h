/**
 * Project   Graviz
 *
 * @file     CCompiler.h
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#pragma once

#include <vector>
#include <string>

#include "framework/Commands/ITerminalCommand.h"
#include "framework/Commands/system/CSystemCmd.h"

namespace NCommand
{

class CCompiler : public ITerminalCommand
{
public: // methods
    CCompiler(QStringList args = QStringList());
    QString getManualMessage() override;

public slots: // methods
    void run() override;
    void terminate() override;

private: // fields
    std::vector<std::string> mFlags;
    std::shared_ptr<CSystemCmd> mProc;
};

} // namespace NCommand
