/**
 * Project   AlgoVi
 *
 * @file     CCommandHandler.h
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#pragma once

#include "algovi/CAlgoViSystem.h"
#include "algovi/Types.h"

namespace NAlgoVi
{

class CAlgoViSystem;

class CCommandHandler : public QObject
{
    Q_OBJECT
public: // methods
    CCommandHandler(CAlgoViSystem* parent);

    void handle(const QString &commandStr);
/*
signals:
    void log(QString msg);
    void error(QString msg);
    void endCommand();
    void endSystemCommand();
private slots:
    void handleLog(QString msg);
    void handleError(QString msg);
    void handleEndCommand();
*/
private: // methods
    bool sysCommand(const QString& command);

private: // fields
    CAlgoViSystem* mParent;
};

} // namespace NAlgoVi

