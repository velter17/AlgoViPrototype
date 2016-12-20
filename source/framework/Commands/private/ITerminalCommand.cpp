/**
 * Project   Graviz
 *
 * @file     ITerminalCommand.cpp
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#include <QDebug>

#include "framework/Commands/ITerminalCommand.h"

namespace NCommand
{

ITerminalCommand::ITerminalCommand(QStringList args)
    : mDirectory("")
    , mOptions("Allowed options")
    , mArgs(args)
    , mTime(60*60*100) // minute

{
    mOptions.add_options()
         ("help,h", "show help")
         ("manual,?", "Short usage manual");
}

ITerminalCommand::~ITerminalCommand()
{
    qDebug () << "~ITerminalCommand()";
}

QString ITerminalCommand::getManualMessage()
{
    return "No manual message<br>"
           "Developer is too lazy...<br>";
}

bool ITerminalCommand::readOptions(const QStringList& args, boost::program_options::variables_map &vm)
{
    try
    {
        std::vector <std::string> vargs;
        for(const QString str : args)
            vargs.push_back(str.toStdString());
        boost::program_options::parsed_options parser =
                boost::program_options::command_line_parser(vargs)
                .options(mOptions)/*.positional(mOptionsPositional)*/.run();
        boost::program_options::store(parser, vm);
    }
    catch(const boost::program_options::error& e)
    {
        emit error(QString::fromLocal8Bit(e.what()));
        return false;
    }
    if(vm.count("help"))
    {
        emit log(getOptionsHelp());
        return false;
    }
    if(vm.count("manual"))
    {
        emit logHtml(getManualMessage());
        return false;
    }
    try
    {
        boost::program_options::notify(vm);
    }
    catch(const boost::program_options::error& e)
    {
        emit error(QString::fromLocal8Bit(e.what()));
        emit log(getOptionsHelp());
        return false;
    }
    return true;
}

void ITerminalCommand::setWorkingDir(const QString &dir)
{
    mDirectory = dir;
}

void ITerminalCommand::setTime(int msecs)
{
    mTime = msecs;
}

QStringList ITerminalCommand::getOptionList()
{
    QStringList ret;
    for(const boost::shared_ptr<boost::program_options::option_description> itr : mOptions.options())
    {
        ret.append(QString::fromStdString(itr->canonical_display_name()));
    }
    return ret;
}


void ITerminalCommand::appendData(const QString &data)
{
    // empty function for overloading
}

QString ITerminalCommand::getOptionsHelp()
{
    std::stringstream sstream;
    sstream << mOptions;
    return QString::fromStdString(sstream.str());
}

} // namespace NCommand
