/**
 * Project   Graviz
 *
 * @file     ITerminalCommand.h
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#pragma once

#include <boost/program_options.hpp>
#include <QStringList>
#include <QObject>
#include <QProcess>
#include <sstream>
#include <QThread>

namespace NCommand
{

class ITerminalCommand : public QThread
{
    Q_OBJECT
public: // methods
    ITerminalCommand(QStringList args = QStringList());
    virtual ~ITerminalCommand();

    virtual QString getManualMessage();

    void setWorkingDir(const QString& dir);

signals:
    void log(QString msg);
    void error(QString msg);
    void logHtml(QString msg);
protected: // methods
   QString getOptionsHelp();
   bool readOptions(const QStringList &args, boost::program_options::variables_map &vm);

protected: // fields
   QString mDirectory;
   boost::program_options::options_description mOptions;
   QStringList mArgs;
};

} // namespace NCommand
