/**
 * Project   Graviz
 *
 * @file     CCommandHandler.cpp
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#include <QDebug>

#include "framework/Commands/system/CCompiler.h"
#include "framework/Commands/CFileSystem.h"
#include "framework/settings/CCommandSettings.h"
#include "../CCommandHandler.h"


namespace NGraviz
{

CCommandHandler::CCommandHandler(CGravizSystem *parent)
    : mParent(parent)
{

}

void CCommandHandler::handle(const QString& commandStr)
{
    //mParent->setMode(TSystemMode::InProcess);
    qDebug () << "CCommandHandler> handle " << commandStr;
    QStringList args = commandStr.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    if(args.empty())
    {
        mParent->handle<TGravizCommand::Empty>(args);
        return;
    }

    QString command = *args.begin();
    args.erase(args.begin());
    if(command == "compile")
    {
        mParent->handle<TGravizCommand::Compile>(args);
    }
    else if(command == "cd")
    {
        mParent->handle<TGravizCommand::ChangeDirectory>(args);
    }
    else if(command == "run")
    {
        mParent->handle<TGravizCommand::RunSolver>(args);
    }
    else if(command == "exit")
    {
        mParent->handle<TGravizCommand::Exit>(args);
    }
    else if(command == "test")
    {
        mParent->handle<TGravizCommand::Test>(args);
    }
    else if(command == "parse")
    {
        mParent->handle<TGravizCommand::ParseSite>(args);
    }
    else if(command == "tester")
    {
        mParent->handle<TGravizCommand::Tester>(args);
    }
    else if(command == "run-visual")
    {
        mParent->handle<TGravizCommand::RunSolverVisual>(args);
    }
    else if(command == "python")
    {
        mParent->handle<TGravizCommand::Python>(QStringList());
    }
    else if(sysCommand(command))
    {
        command = *commandStr.split(QRegExp("\\s+"), QString::SkipEmptyParts).begin();
//        for(const QString& str : args)
//        {
//            command += " " + str;
//        }
        mParent->handle<TGravizCommand::System>(QStringList() << command << args);
    }
    else
    {
        mParent->handle<TGravizCommand::Unknown>(QStringList() << command << args);
    }

    /*
    qDebug () << "CCommandHandler> handle " << commandStr;
    QStringList args = commandStr.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    if(args.empty())
    {
        handleEndCommand();
        return;
    }
    QString command = *args.begin();
    if(command == "compile")
    {
        args.erase(args.begin());
        NCommand::CCompiler* compiler = new NCommand::CCompiler(args);
        connect(compiler, SIGNAL(finished()), compiler, SLOT(deleteLater()));
        connect(compiler, SIGNAL(error(QString)), this, SLOT(handleError(QString)));
        connect(compiler, SIGNAL(log(QString)), this, SLOT(handleLog(QString)));
        if(!system)
        {
            connect(compiler, SIGNAL(finished()), this, SLOT(handleEndCommand()));
        }
        else
        {
            connect(compiler, &NCommand::CCompiler::finished, [this](){emit endSystemCommand();});
        }
        compiler->start();
        //compiler->wait();
        //mController->setSolutionPath(mFileSystem.getCurrentPath() + "/app");
    }
    else if(command == "cd")
    {
        NCommand::CFileSystem::getInstance().changeDir(*(args.begin()+1));
        emit endCommand();
    }
    else if(command == "run")
    {
        mParent->runSolver("");
    }
    else if(sysCommand(command))
    {
        NCommand::CSystemCmd* cmd = new NCommand::CSystemCmd(QStringList() << commandStr);
        connect(cmd, SIGNAL(finished()), cmd, SLOT(deleteLater()));
        connect(cmd, SIGNAL(error(QString)), this, SLOT(handleError(QString)));
        connect(cmd, SIGNAL(log(QString)), this, SLOT(handleLog(QString)));
        connect(cmd, SIGNAL(finished()), this, SLOT(handleEndCommand()));
        cmd->setWorkingDir(NCommand::CFileSystem::getInstance().getCurrentPath());
        cmd->sta)rt();
    }
    else
    {
        emit error("Error. " + command + ": unknown command");
        handleEndCommand();
    }
    */
}
/*
void CCommandHandler::handleLog(QString msg)
{
    emit log(msg);
}

void CCommandHandler::handleError(QString msg)
{
    emit error(msg);
}

void CCommandHandler::handleEndCommand()
{
    emit endCommand();
}
*/

bool CCommandHandler::sysCommand(const QString &command)
{
    return NSettings::CCommandSettings::getInstance().isCommand(command);
}


} // namespace NGraviz
