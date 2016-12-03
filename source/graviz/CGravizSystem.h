/**
 * Project   Graviz
 *
 * @file     CGravizSystem.h
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#pragma once

#include <QObject>
#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QDateTime>

#include "Controller/CSystemController.h"
#include "graviz/CCommandHandler.h"
#include "graviz/Types.h"
#include "graviz/ProblemSolver/CProblemSolver.h"
#include "graviz/ProblemSolver/CProblemCompiler.h"

namespace NController {
class CSystemController;
}

namespace NGraviz
{

class CCommandHandler;

class CGravizSystem : public QObject
{
    Q_OBJECT
public:
    CGravizSystem(std::shared_ptr<NController::CSystemController> controll);

    void handleCommand(NController::TTerminalCommandType type, const QString& cmd);
    void runSolver(QString inputData);


public slots:
    void setMode(TSystemMode mode);

private slots:
    void compileSourceCode(const QString& path);

private:
    std::shared_ptr<NController::CSystemController> mController;
    std::shared_ptr<NView::CGraphicView> mView;
    std::shared_ptr<CCommandHandler> mCommandHandler;
    std::shared_ptr<CProblemSolver> mProblemSolver;
    std::shared_ptr<CProblemCompiler> mProblemCompiler;
    TSystemMode mMode;

    QFileSystemWatcher mFileWatcher;
    QFileInfo mSourceCodeFileInfo;
    QDateTime mLastModified;
};

} // namespace NGraviz
