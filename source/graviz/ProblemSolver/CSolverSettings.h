/**
 * Project   Graviz
 *
 * @file     CSolverSettings.h
 * @author   Dmytro Sadovyi
 * @date     04.11.2016
 */

#pragma once

#include <QString>

#include "graviz/Types.h"

#define DECLARE_ATTRIBUTE(name, type) \
    public:\
        const type& get##name() const\
        { return m##name; }\
        CSolverSettings& set##name(const type& param)\
        { m##name = param; return *this; }\
    private:\
        type m##name;


namespace NGraviz
{

class CSolverSettings
{
public: // methods
    CSolverSettings();

    DECLARE_ATTRIBUTE(SolverAppPath, QString)
    DECLARE_ATTRIBUTE(SourceCodePath, QString)
    DECLARE_ATTRIBUTE(Type, TProblemSolverType)
    DECLARE_ATTRIBUTE(TimeLimit, uint32_t)
    DECLARE_ATTRIBUTE(CheckerPath, QString)
    DECLARE_ATTRIBUTE(CheckerAppPath, QString)
    DECLARE_ATTRIBUTE(NaiveSourcePath, QString)
    DECLARE_ATTRIBUTE(NaiveAppPath, QString)
    DECLARE_ATTRIBUTE(TestGeneratorSourcePath, QString)
    DECLARE_ATTRIBUTE(TestGeneratorAppPath, QString)
};

} // namespace NGraviz

