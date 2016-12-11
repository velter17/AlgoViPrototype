/**
 * Project   Graviz
 *
 * @file     CFileSystem.h
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#pragma once

#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <QStringList>
#include <QObject>

namespace NCommand
{

class CFileSystem : public QObject
{
    Q_OBJECT
public: // methods
    static CFileSystem& getInstance();

    QStringList getDirList();
    QString getCurrentPath();
    void changeDir(const QString& dir);
    QPair<QStringList, int> getHint(const QString& curStr);
    bool isDirectory(const QString& obj);
    bool isFile(const QString& obj);
    boost::filesystem::path getFullPath(const QString& path);

    void remove(const QString& path);
private: // methods
    CFileSystem();
    CFileSystem(const CFileSystem&);
signals:
    void error(const QString& msg);
private: // fields
    boost::filesystem::path mCurrentPath;
};

} // namespace NCommand
