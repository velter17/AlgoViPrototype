#ifndef CFILESYSTEM_H
#define CFILESYSTEM_H

#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <QStringList>
#include <QObject>


class CFileSystem : public QObject
{
    Q_OBJECT
public:
    CFileSystem();
    QStringList getDirList();
    QString getCurrentPath();
    void changeDir(const QString& dir);
    QPair<QStringList, int> getHint(const QString& curStr);
    bool isDirectory(const QString& obj);
signals:
    void error(const QString& msg);
private:
    boost::filesystem::path mCurrentPath;
};

#endif // CFILESYSTEM_H
