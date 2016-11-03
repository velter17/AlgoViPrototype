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
    CFileSystem();
    QStringList getDirList();
    QString getCurrentPath();
    void changeDir(const QString& dir);
    QPair<QStringList, int> getHint(const QString& curStr);
    bool isDirectory(const QString& obj);
signals:
    void error(const QString& msg);
private: // fields
    boost::filesystem::path mCurrentPath;
};

} // namespace NCommand
