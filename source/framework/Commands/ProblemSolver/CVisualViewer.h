/**
 * Project   AlgoVi
 *
 * @file     CVisualSolver.h
 * @author   Dmytro Sadovyi
 * @date     27.02.2017
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

class CVisualViewer : public ITerminalCommand
{
    Q_OBJECT
public: // methods
    CVisualViewer(const QStringList& args, std::shared_ptr<CCompilerHandler> compilerHandler);

    bool init();

public slots: //methods
    void run() override;
    void terminate() override;
    void appendData(const QString& data) override;
//    void setInput(const QString& data);
    QString getViewerScriptPath();

signals:
    void command(const QString& cmd);
private:
    QString mViewerScriptPath;
    boost::program_options::variables_map mVarMap;
};

} // namespace NCommand

