/**
 * Project   AlgoVi
 *
 * @file     CVisualSolverImpl.h
 * @author   Dmytro Sadovyi
 * @date     17.12.2016
 */

#pragma once

#include "framework/Commands/ProblemSolver/CCompilerHandler.h"
#include "framework/Commands/ProblemSolver/CProblemSolver.h"

namespace NCommand
{

class CVisualSolverImpl : public QObject
{
    Q_OBJECT
public: // methods
    CVisualSolverImpl(std::shared_ptr<CProblemSolver> solver,
                      std::shared_ptr<CCompilerHandler> compilerHandler);

public slots:
    void solve();

signals:
    void started();
    void finished(int code);

private:
    std::shared_ptr<CProblemSolver> mSolver;
    std::shared_ptr<CCompilerHandler> mCompilerHandler;
};

} // namespace NCommand

