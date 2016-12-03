/**
 * Project   Graviz
 *
 * @file     ITerminal.h
 * @author   Dmytro Sadovyi
 * @date     02.12.2016
 */

#pragma once

#include <QObject>
#include <QWidget>

#include "Types.h"

namespace NController
{

class ITerminal
{

public slots: // methods
    virtual void appendOutput(const QString& str) = 0;
    virtual void appendError(const QString& str) = 0;
    virtual void question(const QString& str, const std::vector <QString> &answers) = 0;
    virtual void lock() = 0;
    virtual void unlock() = 0;
    virtual void appMode() = 0;

signals:
    virtual void command(TTerminalCommandType type, const QString& cmd) = 0;

protected:
    TTerminalState mState;
};

} // namespace NController

