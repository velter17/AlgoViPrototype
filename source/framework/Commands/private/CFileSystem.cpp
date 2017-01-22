/**
 * Project   Graviz
 *
 * @file     CFileSystem.cpp
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#include <QDebug>
#include <sstream>

#include "framework/Commands/CFileSystem.h"

namespace NCommand
{

CFileSystem::CFileSystem()
    : mCurrentPath(boost::filesystem::path(getenv("HOME")))
{
}

CFileSystem::CFileSystem(const CFileSystem &)
{
}

CFileSystem &CFileSystem::getInstance()
{
    static CFileSystem sys;
    return sys;
}

QStringList CFileSystem::getDirList()
{
    QStringList ret;
    if(!boost::filesystem::exists(mCurrentPath))
    {
        qDebug () << mCurrentPath.c_str() << "doesn't exist";
    }
    boost::filesystem::directory_iterator end_itr;
    for(boost::filesystem::directory_iterator itr(mCurrentPath);
        itr != end_itr; ++itr)
    {
        const boost::filesystem::directory_entry& entry = *itr;
        if(entry.path().filename().c_str()[0] == '.')
            continue;
        ret.append(QString::fromStdString(entry.path().filename().generic_string()));
    }
    return ret;
}

QString CFileSystem::getCurrentPath()
{
    return QString::fromStdString(mCurrentPath.string());
}

void CFileSystem::changeDir(const QString &dir)
{
    std::string toDir = dir.toStdString();
    if(toDir.size() > 1 && toDir.back() == '/')
        toDir.pop_back();
#ifdef WIN_TARGET
    if(toDir.length() > 1 && toDir[1] == ':')
#else
    if(toDir[0] == '/')
#endif
    {
        if(!boost::filesystem::exists(toDir))
        {
            emit error(" [ Error ] " + dir + ": no such path\n");
            return;
        }
        mCurrentPath = toDir;
    }
    else
    {
        if(!boost::filesystem::exists(mCurrentPath / toDir))
        {
            emit error(" [ Error ] " + dir + " : no such file or directory\n");
            return;
        }
        else if(!boost::filesystem::is_directory(mCurrentPath / toDir))
        {
            emit error(" [ Error ] " + dir + ": not a directory\n");
            return;
        }
        mCurrentPath = (mCurrentPath / toDir).normalize();
    }
}

QPair<QStringList, int> CFileSystem::getHint(const QString &curStr)
{
    boost::filesystem::path p;
#ifdef WIN_TARGET
    if(!(curStr.length() > 1 && curStr[1] == ':'))
#else
    if(curStr[0] != '/')
#endif
        p = (mCurrentPath / curStr.toStdString());
    else
        p = curStr.toStdString();
    p.normalize();
    QString file_name = QString::fromStdString(p.filename().generic_string());
    if(file_name == ".")
        file_name.clear();
    p = p.parent_path();
    if(!boost::filesystem::exists(p))
    {
        qDebug () << p.c_str() << " is not a path";
        return QPair<QStringList, int>(QStringList(), 0);
    }
    qDebug() << "path: " << p.c_str() << ", file: " << file_name;
    QStringList ret;
    boost::filesystem::directory_iterator end_itr;
    for(boost::filesystem::directory_iterator itr(p);
        itr != end_itr; ++itr)
    {
        const boost::filesystem::directory_entry& entry = *itr;
        if(entry.path().filename().c_str()[0] == '.')
            continue;
        if(!file_name.isEmpty() &&
                strncmp((char*)entry.path().filename().c_str(), file_name.toLocal8Bit(), file_name.length()))
            continue;
        ret.append(QString::fromStdString(entry.path().filename().string()));
    }
    return QPair<QStringList, int>(ret, file_name.length());
}

bool CFileSystem::isDirectory(const QString& obj)
{
    boost::filesystem::path p;
#ifdef WIN_TARGET
    if(!(obj.length() > 1 && obj[1] == ':'))
#else
    if(obj[0] != '/')
#endif
        p = mCurrentPath / obj.toStdString();
    else
        p = obj.toStdString();
    qDebug () << p.c_str() << " is a directory?";
    return boost::filesystem::is_directory(p);
}

bool CFileSystem::isFile(const QString &obj)
{
    boost::filesystem::path p;
#ifdef WIN_TARGET
    if(!(obj.length() > 1 && obj[1] == ':'))
#else
    if(obj[0] != '/')
#endif
        p = mCurrentPath / obj.toStdString();
    else
        p = obj.toStdString();
    qDebug () << "CFileSystem> isFile (" << obj << ") -> " << p.c_str();
    return boost::filesystem::is_regular_file(p);
}

bool CFileSystem::exists(const QString &path)
{
    boost::filesystem::path p;
#ifdef WIN_TARGET
    if(!(path.length() > 1 && path[1] == ':'))
#else
    if(path[0] != '/')
#endif
        p = mCurrentPath / path.toStdString();
    else
        p = path.toStdString();
    qDebug () << "CFileSystem> exists (" << path << ") -> " << p.c_str();
    return boost::filesystem::exists(p);
}

boost::filesystem::path CFileSystem::getFullPath(const QString &path)
{
    boost::filesystem::path p;
#ifdef WIN_TARGET
    if(!(path.length() > 1 && path[1] == ':'))
#else
    if(path[0] != '/')
#endif
        p = mCurrentPath / path.toStdString();
    else
        p = path.toStdString();
    p.normalize();
    return boost::filesystem::complete(p);
}

void CFileSystem::remove(const QString &path)
{
    boost::filesystem::remove(path.toStdString());
}

} // namespace NCommand
