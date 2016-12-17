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
#include <QDebug>

namespace NCommand
{

class ITerminalCommand : public QObject
{
    Q_OBJECT
public: // methods
    ITerminalCommand(QStringList args = QStringList());
    virtual ~ITerminalCommand() = 0;

    virtual QString getManualMessage();

    void setWorkingDir(const QString& dir);

    /*
     * set maxTime of execution
    */
    void setTime(int msecs);

    QStringList getOptionList();


public slots:

    /*
     * Start point of command
    */
    virtual void run() = 0;

    /*
     * Method for communication with process
    */
    virtual void appendData(const QString& data);

    virtual void terminate() = 0;

signals:
    void log(QString msg);
    void error(QString msg);
    void logHtml(QString msg);
    void started();
    void finished(int code);
protected: // methods
   QString getOptionsHelp();
   bool readOptions(const QStringList &args, boost::program_options::variables_map& vm);
   bool operateOptions();


protected: // fields
   QString mDirectory;
   boost::program_options::options_description mOptions;
   boost::program_options::positional_options_description mOptionsPositional;
   QStringList mArgs;
   int mTime;
};

} // namespace NCommand
