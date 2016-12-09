/**
 * Project   Graviz
 *
 * @file     CSolverSettings.cpp
 * @author   Dmytro Sadovyi
 * @date     04.11.2016
 */

#include "../CSolverSettings.h"

namespace NGraviz
{

CSolverSettings::CSolverSettings()
    : mType(TProblemSolverType::Straight)
    , mTimeLimit(0)
{

}


} // namespace NGraviz
