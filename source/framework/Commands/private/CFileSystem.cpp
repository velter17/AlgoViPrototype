#include "framework/Commands/CFileSystem.h"
#include <QDebug>
#include <sstream>

CFileSystem::CFileSystem()
    : mCurrentPath("/home/dsadovyi")
{

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
    if(!boost::filesystem::exists(mCurrentPath / dir.toStdString()))
    {
        emit error(dir + ": no such file or directory");
        return;
    }
    else if(!boost::filesystem::is_directory(mCurrentPath / dir.toStdString()))
    {
        emit error(dir + ": not a directory");
        return;
    }
    mCurrentPath = (mCurrentPath / dir.toStdString()).normalize();
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


