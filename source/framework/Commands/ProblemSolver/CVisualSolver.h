/**
 * Project   Graviz
 *
 * @file     CVisualSolver.h
 * @author   Dmytro Sadovyi
 * @date     16.12.2016
 */

#pragma once

#include <QObject>
#include <QProcess>
#include <QDebug>
#include <vector>
#include <string>
#include <thread>
#include <functional>
#include <mutex>

#include "framework/Commands/ProblemSolver/CCompilerHandler.h"
#include "framework/Commands/ITerminalCommand.h"
#include "framework/Commands/ProblemSolver/CProblemSolver.h"

namespace NCommand
{

class CVisualSolver : public ITerminalCommand
{
    Q_OBJECT
public: // methods
    CVisualSolver(const QStringList& args, std::shared_ptr<CCompilerHandler> compilerHandler);

    bool init();

public slots: //methods
    void run() override;
    void terminate() override;
    void appendData(const QString& data) override;
    void setInput(const QString& data);
    QString getAlgoScriptPath();

signals:
    void command(const QString& cmd);

private: // methods
    void startSolver();
    void runSolver();

private: // fields
    std::shared_ptr<CCompilerHandler> mCompilerHandler;
    std::vector<std::string> mFlagParsed;
    QStringList mFlags;
    boost::program_options::variables_map mVarMap;
    std::shared_ptr<CProblemSolver> mSolver;
    QString mSourceCode;
    QString mAlgoScript;
    QString mInput;
    int mInputTimestamp;
    int mLastTimestamp;
    bool mSolvedFlag;
    bool mCompiledFlag;
    QString mOutput;
};

} // namespace NCommand

