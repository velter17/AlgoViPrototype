/**
 * Project   Graviz
 *
 * @file     CProblemSolver.h
 * @author   Dmytro Sadovyi
 * @date     04.11.2016
 */

#pragma once

#include <QObject>
#include <QProcess>

#include "graviz/ProblemSolver/CSolverSettings.h"

namespace NGraviz
{

class CProblemSolver : public QObject
{
    Q_OBJECT
public: // methods
    CProblemSolver(const CSolverSettings& settings = CSolverSettings());


public slots:
    void solve(QString inputData);
    void terminate();

    void appendData(QString data);

    CSolverSettings getSettings() const;
    void setSettings(const CSolverSettings& settings);

signals:
    /**
     * @brief out
     * emitted when solver flushed to output stream
     */
    void out(QString);

    /**
     * @brief err
     * emitted when solver flushed to error stream
     */
    void err(QString);

    /**
     * @brief started
     * emitted when solver start to run execution
     */
    void started();

    /**
     * @brief finished
     * emitted when solver finished execution
     * param1 - result code
     *      0 - success
     *      1 - failure
     *      2 - runtime error
     * param2 - checker message
     */
    void finished(int, QString);
private: // fields
    CSolverSettings mSettings;
    int mSettingsTimestamp;
    int mSettingsUpdatedTimestamp;
    //std::shared_ptr<QProcess> mApp;
    QProcess* mApp;
};

} // namespace NGraviz

