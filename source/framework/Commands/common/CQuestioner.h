/**
 * Project   Graviz
 *
 * @file     CQuestioner.h
 * @author   Dmytro Sadovyi
 * @date     09.12.2016
 */

#pragma once

#include "framework/Commands/ITerminalCommand.h"

namespace NCommand
{

class CQuestioner : public ITerminalCommand
{
public: // methods
    CQuestioner(const QStringList& args);

    void run() override;
    void appendData(const QString &data) override;
    void terminate() override;

private: // fields
    std::vector<std::string> mOptionsVector;
    std::string mQuestion;
};

} // namespace NCommand

