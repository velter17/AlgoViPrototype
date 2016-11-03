/**
 * Project   Graviz
 *
 * @file     CCompiler.cpp
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#include <boost/program_options.hpp>
#include <QDebug>
#include <QString>
#include <iostream>

#include "framework/Commands/system/CCompiler.h"

namespace NCommand
{

CCompiler::CCompiler(QStringList args)
    : ITerminalCommand(args)
{
    mOptions.add_options()
        ("src,s", boost::program_options::value<std::string>()->required(), "path to source file")
        ("lang,l", boost::program_options::value<std::string>()->default_value("c++"), "language");
}

void CCompiler::run()
{
    boost::program_options::variables_map vm;
    if(!readOptions(mArgs, vm))
    {
        return;
    }
    QString src = QString::fromStdString(vm["src"].as<std::string>());
    emit log("Processing.....");
    CSystemCmd* proc = new CSystemCmd(QStringList() << "g++ -std=c++11 -o app " + src);
    connect(proc, SIGNAL(finished()), proc, SLOT(deleteLater()));
    connect(proc, &CCompiler::log,
            [this](QString msg){emit log(msg);});
    connect(proc, &CCompiler::error,
            [this](QString msg){emit error(msg);});
    proc->setWorkingDir(this->mDirectory);
    proc->start();
    proc->wait();
}

QString CCompiler::getManualMessage()
{
    return "<br><font color=yellow><b>Compiler</b></font><br><br>"
           "<font color=white>"
           "Command for source code compilation<br>"
           "Application will be automatically used as task solver<br>"
           "See help for options description (-h, --help)"
           "</font>";
}

} // namespace NCommand
