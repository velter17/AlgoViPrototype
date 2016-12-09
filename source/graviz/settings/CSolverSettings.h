/**
 * Project   Graviz
 *
 * @file     CSolverSettings.h
 * @author   Dmytro Sadovyi
 * @date     04.11.2016
 */

#pragma once

#include <QString>
#include <QStringList>

#include "graviz/Types.h"
#include "graviz/settings/HelperFunctions.h"


namespace NGraviz
{

DECLARE_CLASS_HEAD(SolverSettings)

DECLARE_ATTRIBUTE(SolverSettings, SolverAppPath, QStringList)
DECLARE_ATTRIBUTE(SolverSettings, SourceCodePath, QStringList)
DECLARE_ATTRIBUTE(SolverSettings, Type, TProblemSolverType)
DECLARE_ATTRIBUTE(SolverSettings, TimeLimit, uint32_t)
DECLARE_ATTRIBUTE(SolverSettings, CheckerPath, QString)
DECLARE_ATTRIBUTE(SolverSettings, CheckerAppPath, QString)
DECLARE_ATTRIBUTE(SolverSettings, TestGeneratorSourcePath, QString)
DECLARE_ATTRIBUTE(SolverSettings, TestGeneratorAppPath, QString)

DECLARE_CLASS_END

} // namespace NGraviz

