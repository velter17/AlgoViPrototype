/**
 * Project   Graviz
 *
 * @file     CCSolverAppBase.h
 * @author   Dmytro Sadovyi
 * @date     04.11.2016
 */

#pragma once

#include <QProcess>

namespace NSolver
{

class CSolverAppBase : public QProcess
{
public: // methods
    CSolverAppBase(const QString appName, const QStringList& args = QStringList());

};

} // namespace NSolver

