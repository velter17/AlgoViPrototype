/**
 * Project   AlgoVi
 *
 * @file     CVisualSolverImpl.cpp
 * @author   Dmytro Sadovyi
 * @date     17.12.2016
 */

#include "../CVisualSolverImpl.h"

namespace NCommand
{

CVisualSolverImpl::CVisualSolverImpl(
        std::shared_ptr<CProblemSolver> solver,
        std::shared_ptr<CCompilerHandler> compilerHandler)
    : mSolver(solver)
    , mCompilerHandler(compilerHandler)
{

}

void CVisualSolverImpl::solve()
{

}


} // namespace NCommand
