#ifndef CTERMINALCOMMAND_H
#define CTERMINALCOMMAND_H

#include <boost/program_options.hpp>
#include <QStringList>
#include <QObject>
#include <QProcess>
#include <sstream>
#include <QThread>

class ITerminalCommand : public QThread
{
    Q_OBJECT
public:
    ITerminalCommand(QStringList args = QStringList());
    virtual ~ITerminalCommand();

    virtual QString getManualMessage();

    void setWorkingDir(const QString& dir);

signals:
    void log(QString msg);
    void error(QString msg);
    void logHtml(QString msg);
protected:
   QString getOptionsHelp();
   bool readOptions(const QStringList &args, boost::program_options::variables_map &vm);

protected:
   QString mDirectory;
   boost::program_options::options_description mOptions;
   QStringList mArgs;
};


#endif // CTERMINALCOMMAND_H
