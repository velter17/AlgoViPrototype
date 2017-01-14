/**
 * Project   Graviz
 *
 * @file     CCompilerHandler.cpp
 * @author   Dmytro Sadovyi
 * @date     07.11.2016
 */

#include "../CCompilerHandler.h"

namespace NCommand
{

CCompilerHandler::CCompilerHandler()
    : mAppIdx(1)
{
    mTempDir.setAutoRemove(true);
}

void CCompilerHandler::addSourceCodePath(const QString &path)
{
    QString fullPath = QString::fromStdString(CFileSystem::getInstance().getFullPath(path).generic_string());
    QString appName = mTempDir.path() + "/" + QString("app%1").arg(mAppIdx++);
    mSourceCodeFileMap[fullPath] = SFileInfo(fullPath, appName);
}

bool CCompilerHandler::isSourceCode(const QString &path)
{
    return mSourceCodeFileMap.find(path) != mSourceCodeFileMap.end();
}

bool CCompilerHandler::isNeededCompilation(const QString &path)
{
    QString fullPath = QString::fromStdString(CFileSystem::getInstance().getFullPath(path).generic_string());
    return mSourceCodeFileMap[fullPath].isModified();
}

void CCompilerHandler::performCompilation(const QString& path, const QStringList &args)
{
    qDebug () << "CCompilerHandler> performCompilation: " << path << " " << args;
    QString fullPath = QString::fromStdString(CFileSystem::getInstance().getFullPath(path).generic_string());
    NCommand::CFileSystem::getInstance().remove(getAppPath(fullPath));

    CCompiler* compiler = new CCompiler(
                QStringList() << "-i" << fullPath
                              << "-o" << getAppPath(fullPath)
                              << args);
    compiler->setTime(60*60*10);
    connect(compiler, &CCompiler::finished, [this, compiler, fullPath](int code)
    {
        qDebug () << "CCompilerHandler> compilation was finished with code " << code;
        if(code == 0)
        {
            mSourceCodeFileMap[fullPath].setUpToDate();
        }
        emit finished(code);
        compiler->deleteLater();
    });
    connect(compiler, &CCompiler::log, [this](const QString& log){
        emit out(log);
    });
    connect(compiler, &CCompiler::error, [this](const QString& log){
        emit err(log);
    });
    emit started();
    emit out(" [ Info ] Compilation...\n");
    compiler->run();
}

QString CCompilerHandler::getAppPath(const QString &codePath)
{
    return mSourceCodeFileMap[codePath].mBinaryFilePath;
}

void CCompilerHandler::clear()
{
    mSourceCodeFileMap.clear();
}



SFileInfo::SFileInfo()
{

}

SFileInfo::SFileInfo(const QString &path, const QString &binaryPath)
{
    mLastModified = QDateTime();
    mFileInfo.setFile(path);
    mBinaryFilePath = binaryPath;
}

bool SFileInfo::isModified()
{
    mFileInfo.refresh();
    return mLastModified.toTime_t() != mFileInfo.lastModified().toTime_t();
}

void SFileInfo::setUpToDate()
{
    mLastModified = mFileInfo.lastModified();
}


} // namespace NGraviz
