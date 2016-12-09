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
        ret.append(QString::fromLocal8Bit(entry.path().filename().c_str()));
    }
    return ret;
}

QString CFileSystem::getCurrentPath()
{
    return QString(mCurrentPath.c_str());
}

void CFileSystem::changeDir(const QString &dir)
{
    std::string toDir = dir.toStdString();
    if(toDir.size() > 1 && toDir.back() == '/')
        toDir.pop_back();
    if(toDir.front() == '/')
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
    if(curStr[0] != '/')
        p = (mCurrentPath / curStr.toStdString());
    else
        p = curStr.toStdString();
    p.normalize();
    QString file_name = QString::fromLocal8Bit(p.filename().c_str());
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
                strncmp(entry.path().filename().c_str(), file_name.toLocal8Bit(), file_name.length()))
            continue;
        ret.append(QString::fromLocal8Bit(entry.path().filename().c_str()));
    }
    return QPair<QStringList, int>(ret, file_name.length());
}

bool CFileSystem::isDirectory(const QString& obj)
{
    boost::filesystem::path p;
    if(obj[0] != '/')
        p = mCurrentPath / obj.toStdString();
    else
        p = obj.toStdString();
    return boost::filesystem::is_directory(p);
}

boost::filesystem::path CFileSystem::getFullPath(const QString &path)
{
    boost::filesystem::path p;
    if(path[0] != '/')
        p = mCurrentPath / path.toStdString();
    else
        p = path.toStdString();
    return boost::filesystem::complete(p);
}

void CFileSystem::remove(const QString &path)
{
    boost::filesystem::remove(path.toStdString());
}

} // namespace NCommand
