/**
 * Project   AlgoVi
 *
 * @file     CProblemSolver.h
 * @author   Dmytro Sadovyi
 * @date     04.11.2016
 */

#pragma once

#include <QObject>
#include <QProcess>
#include <QDebug>
#include <vector>
#include <string>
#include <fstream>

#include "framework/Commands/ProblemSolver/CCompilerHandler.h"
#include "framework/Commands/ITerminalCommand.h"
#include "framework/Commands/ProblemSolver/CTestProvider.h"

namespace NCommand
{

class CProblemSolver : public ITerminalCommand
{
public: // methods
    CProblemSolver(const QStringList& args, std::shared_ptr<CTestProvider> testProvider);

    QString getSourceCodePath();
    const QStringList& getCompilationFlags();

    bool init();
    bool saveTestFlag();
    void setAppPath(const QString& path);

public slots:
    void run() override;
    void terminate() override;
    void sendEOF();
    void appendData(const QString& data) override;

private: // fields
    QString mOptionError;
    //std::shared_ptr<QProcess> mApp;
    boost::program_options::variables_map mVarMap;
    QProcess* mApp;
    std::vector<std::string> mFlagParsed;
    std::vector<std::string> mArgsForApp;
    QStringList mFlags;
    QStringList mArgsForAppList;
    QString mAppPath;
    bool mTestSaveFlag;
    std::ofstream mOutputFile;

    int mTestToExecute;
    bool mTestToExecuteFlag;
    std::shared_ptr<CTestProvider> mTestProvider;
};

} // namespace NCommand

