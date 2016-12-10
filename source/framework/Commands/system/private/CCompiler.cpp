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

#include "framework/Commands/CFileSystem.h"
#include "framework/Commands/system/CCompiler.h"

namespace NCommand
{

CCompiler::CCompiler(QStringList args)
    : ITerminalCommand(args)
    , mFlags({"std=c++11", "O2"})
{
    mOptions.add_options()
        ("input,i", boost::program_options::value<std::string>()->required(), "path to source file")
        //("lang,l", boost::program_options::value<std::string>()->default_value("c++"), "language")
        ("output,o,", boost::program_options::value<std::string>()->required(), "destination path")
        ("flag,f", boost::program_options::value<std::vector<std::string> >()->multitoken(),
            "compilation flags\n(for c++ -DVAL you should use -f VAL");
}

void CCompiler::run()
{
    emit started();
    boost::program_options::variables_map vm;
    if(!readOptions(mArgs, vm))
    {
        emit finished(0);
        return;
    }

    QString src = QString::fromStdString(vm["input"].as<std::string>());
    QString compilerCommand = QString("g++ -o ") +
            CFileSystem::getInstance().getFullPath(QString::fromStdString(vm["output"].as<std::string>())).c_str();
    if(vm.count("flag"))
    {
        for(const std::string& f : vm["flag"].as<std::vector<std::string>>())
        {
            mFlags.push_back(f);
        }
    }
    for(const std::string& f : mFlags)
    {
        compilerCommand += QString(" -") + f.c_str();
    }
    compilerCommand += QString(" ") + CFileSystem::getInstance().getFullPath(src).c_str();
    qDebug () << "compiler command is " << compilerCommand;
    mProc.reset(new CSystemCmd(QStringList() << compilerCommand));
    //connect(proc.get(), SIGNAL(finished()), proc.get(), SLOT(deleteLater()));
    connect(mProc.get(), &CCompiler::log,
            [this](QString msg){emit log(msg);});
    connect(mProc.get(), &CCompiler::error,
            [this](QString msg){emit error(msg);});
    connect(mProc.get(), &CSystemCmd::finished, [this](int code){emit finished(code);});
    mProc->setWorkingDir(this->mDirectory);
    mProc->run();
}

void CCompiler::terminate()
{
    mProc->terminate();
}

QString CCompiler::getManualMessage()
{
    return "<br><font color=yellow><b>Compiler</b></font><br><br>"
           "<font color=white>"
           "Command for source code compilation<br>"
           "See help for options description (-h, --help)"
           "</font>";
}

} // namespace NCommand
