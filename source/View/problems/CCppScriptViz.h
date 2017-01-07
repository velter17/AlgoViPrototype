/**
 * Project   Graviz
 *
 * @file     CCppScriptViz.h
 * @author   Dmytro Sadovyi
 * @date     02.01.2017
 */

#pragma once

#include <QObject>
#include <QMutex>

#include "View/problems/IProblemVisualizer.h"
#include "framework/Commands/CFileSystem.h"
#include "framework/Commands/ProblemSolver/CCompilerHandler.h"
#include "framework/Commands/ProblemSolver/CProblemSolver.h"

namespace NView
{

class CQueryHandler : public QObject
{
    Q_OBJECT
public: // methods
    void queryHandler(QString query,
                      const std::map<QString, IGravizItem *> &items,
                      QPainter* painter = nullptr);

signals:
    void data(const QString& data);
};

class CCppScriptViz : public QObject, public IProblemVisualizer
{
    Q_OBJECT
public: // methods
    CCppScriptViz(std::shared_ptr<NCommand::CCompilerHandler> compilerHandler,
                  QString scriptPath);

    QString serialize(const std::map<QString, IGravizItem *> &items) override;
    void realize(QString &data,
                 QPainter *painter,
                 const std::map <QString, IGravizItem*>& items) override;

    bool isFree();

private: // fields
    QProcess* mProc;
    std::shared_ptr<NCommand::CCompilerHandler> mCompilerHandler;
    CQueryHandler* mQueryHandler;
    QString mScriptPath;
    QString mBuffer;
    QMutex mMutex;
};

} // namespace NView

