/**
 * Project   AlgoVi
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
#include "framework/settings/CSolverSettings.h"

namespace NCommand
{

CCompiler::CCompiler(QStringList args)
    : ITerminalCommand(args)
    //, mFlags({"std=c++11", "O2", "w"})
{
    mOptions.add_options()
        ("input,i", boost::program_options::value<std::string>()->required(), "path to source file")
        //("lang,l", boost::program_options::value<std::string>()->default_value("c++"), "language")
        ("output,o,", boost::program_options::value<std::string>()->required(), "destination path")
        ("flag,f", boost::program_options::value<std::vector<std::string> >()->multitoken(),
            "compilation flags\n(for c++ -DVAL you should use -f DVAL");
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
    qDebug () << "inside compiler";

    QString src = QString::fromStdString(vm["input"].as<std::string>());
    QStringList compilerCommand;
//    compilerCommand << "g++";
//    compilerCommand << "-o" << QString::fromStdString(CFileSystem::getInstance().getFullPath(
//                               QString::fromStdString(vm["output"].as<std::string>())).string());
    QString compilerStr = NSettings::CSolverSettings::getInstance().getCompiler(
                src.endsWith("cpp") ? "cpp" : "java"
                );
    compilerStr.replace("$SRC_PATH$", src);
    compilerStr.replace("$BIN_PATH$", QString::fromStdString(CFileSystem::getInstance().getFullPath(
                                                      QString::fromStdString(vm["output"].as<std::string>())).string()));
    qDebug () << "compilerStr is " << compilerStr;
    compilerCommand << compilerStr.split(" ");
    if(vm.count("flag"))
    {
        for(const std::string& f : vm["flag"].as<std::vector<std::string>>())
        {
            mFlags.push_back(f);
        }
    }
    for(const std::string& f : mFlags)
    {
        compilerCommand << QString("-") + f.c_str();
        //compilerCommand += QString(" -") + f.c_str();
    }
    //compilerCommand += QString(" ") + CFileSystem::getInstance().getFullPath(src).string().c_str();
    //compilerCommand << CFileSystem::getInstance().getFullPath(src).string().c_str();
    qDebug () << "compiler command is " << compilerCommand;
    mProc.reset(new CSystemCmd(compilerCommand));
    //connect(proc.get(), SIGNAL(finished()), proc.get(), SLOT(deleteLater()));
    connect(mProc.get(), &CCompiler::log,
            [this](QString msg){
        qDebug () << "CCompiler> log " << msg;
        emit log(msg);
    });
    connect(mProc.get(), &CCompiler::error,
            [this](QString msg){
        qDebug () << "CCompiler> error " << msg;
        emit error(msg);
    });
    connect(mProc.get(), &CSystemCmd::finished, [this](int code){
        qDebug () << "CCompiler> finished";
        emit finished(code);
    });
    mProc->setWorkingDir(this->mDirectory);
    mProc->run();
    qDebug () << "CCompiler> started";
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
