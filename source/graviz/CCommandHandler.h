/**
 * Project   Graviz
 *
 * @file     CCommandHandler.h
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#pragma once

#include "graviz/CGravizSystem.h"
#include "graviz/Types.h"

namespace NGraviz
{

class CGravizSystem;

class CCommandHandler : public QObject
{
    Q_OBJECT
public: // methods
    CCommandHandler(CGravizSystem* parent);

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
    CGravizSystem* mParent;
};

} // namespace NGraviz

