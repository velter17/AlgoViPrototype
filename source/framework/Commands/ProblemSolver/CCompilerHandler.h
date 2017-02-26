/**
 * Project   AlgoVi
 *
 * @file     CCompilerHandler.h
 * @author   Dmytro Sadovyi
 * @date     07.11.2016
 */

#pragma once

#include <map>
#include <QStringList>
#include <QFileInfo>
#include <QDateTime>
#include <QTemporaryDir>
#include <QDebug>

#include "framework/Commands/CFileSystem.h"
#include "framework/Commands/system/CCompiler.h"

namespace NCommand
{

struct SFileInfo
{
    QFileInfo mFileInfo;
    QDateTime mLastModified;
    QString mBinaryFilePath;
    QString mExecCmd;

    SFileInfo();
    SFileInfo(const QString& path,
              const QString& binaryPath,
              const QString& execCmd);

    bool isModified();
    void setUpToDate();
};

class CCompilerHandler : public QObject
{
    Q_OBJECT
public: // methods
    CCompilerHandler();
    ~CCompilerHandler();

    void addSourceCodePath(const QString& path);
    bool isSourceCode(const QString& path);
    bool isNeededCompilation(const QString& path);
    void performCompilation(const QString &path, const QStringList& args);
    QString getAppPath(const QString& codePath);
    QString getExecCmd(const QString& codePath);

    void clear();

private: // methods
    QString getPathFromArgs(const QStringList& args);

signals:
    void started();
    void out(const QString& log);
    void err(const QString& log);
    void finished(int code);

private: // methods
    std::map<QString, SFileInfo> mSourceCodeFileMap;
    QTemporaryDir mTempDir;
    int mAppIdx;
};

} // namespace NCommand

