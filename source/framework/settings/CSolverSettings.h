/**
 * Project   Graviz
 *
 * @file     CSolverSettings.h
 * @author   Dmytro Sadovyi
 * @date     28.01.2017
 */

#pragma once

#include <boost/preprocessor/punctuation/comma.hpp>

#include <QString>
#include <QPair>
#include <QtXml>

#include "HelperFunctions.h"


namespace NSettings
{

DECLARE_CLASS_HEAD(SolverSettings, "configs/solverConfig.xml")

    DECLARE_ATTRIBUTE_MAP(SolverSettings, Compiler, QString, QString)
    DECLARE_ATTRIBUTE_MAP(SolverSettings, Solver, QString, QPair<QString BOOST_PP_COMMA() bool>)

public: // methods
    QStringList getSolverList();

private: // methods
    void parseCompilers(QDomNode rootNode);
    void parseSolvers(QDomNode rootNode);

DECLARE_CLASS_END()

} // namespace NSettings

