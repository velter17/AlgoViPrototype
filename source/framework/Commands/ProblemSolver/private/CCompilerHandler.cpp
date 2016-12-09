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
{

}

void CCompilerHandler::addSourceCodePath(const QString &path)
{
    QString fullPath = QString::fromStdString(CFileSystem::getInstance().getFullPath(path).generic_string());
    mSourceCodeFileMap[fullPath] = SFileInfo(fullPath);
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

    CCompiler* compiler = new CCompiler(
                QStringList() << "-i" << fullPath
                              << "-o" << "/home/dsadovyi/Coding/app"
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

void CCompilerHandler::clear()
{
    mSourceCodeFileMap.clear();
}

//QString CCompilerHandler::getPathFromArgs(const QStringList &args)
//{
//    std::vector <std::string> vargs;
//    for(const QString& str : args)
//        vargs.push_back(str.toStdString());
//    boost::program_options::variables_map;
//    boost::program_options::parsed_options parser = boost::program_options::command_line_parser(vargs).options(mOptions).run();
//    boost::program_options::store(parser, vm);
//    if(vm.count['input'])

//}



SFileInfo::SFileInfo()
{

}

SFileInfo::SFileInfo(const QString &path)
{
    mLastModified = QDateTime();
    mFileInfo.setFile(path);
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
