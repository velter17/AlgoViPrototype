/**
 * Project   Graviz
 *
 * @file     CCommandHandler.h
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#pragma once

#include "graviz/CGravizSystem.h"

namespace NGraviz
{

class CGravizSystem;

class CCommandHandler : public QObject
{
    Q_OBJECT
public: // methods
    CCommandHandler(CGravizSystem* parent);

    void handle(const QString &commandStr);

signals:
    void log(QString msg);
    void error(QString msg);
    void logHtml(QString msg);
    void endCommand();
private slots:
    void handleLog(QString msg);
    void handleError(QString msg);
    void handleHtml(QString msg);
    void handleEndCommand();

private: // fields
    CGravizSystem* mParent;
};

} // namespace NGraviz

