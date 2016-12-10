/**
 * Project   Graviz
 *
 * @file     IProblemParser.h
 * @author   Dmytro Sadovyi
 * @date     10.12.2016
 */

#pragma once

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include "framework/Commands/ITerminalCommand.h"
#include "framework/Commands/ProblemSolver/CTestProvider.h"

namespace NCommand
{

class IProblemParser : public ITerminalCommand
{
public: // methods
    IProblemParser(const QStringList& args);

    void run() override;
    void appendData(const QString &data) override;
    void terminate() override;

    virtual int parse(const QString& path) = 0;

    const std::vector<STest> tests() const;

protected: // fields
    std::vector<STest> mTests;
};

} // namespace NCommand

