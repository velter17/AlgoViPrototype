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
#include "../CCommandHandler.h"

#include "graviz/ProblemSolver/CProblemSolver.h"

namespace NGraviz
{

CCommandHandler::CCommandHandler(CGravizSystem *parent)
    : mParent(parent)
{

}

void CCommandHandler::handle(const QString& commandStr, bool system)
{
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
        if(!system)
        {
            connect(compiler, SIGNAL(error(QString)), this, SLOT(handleError(QString)));
            connect(compiler, SIGNAL(log(QString)), this, SLOT(handleLog(QString)));
            connect(compiler, SIGNAL(finished()), this, SLOT(handleEndCommand()));
        }
        else
        {
            connect(compiler, &NCommand::CCompiler::finished, [this](){emit endSystemCommand();});
        }
        //compiler->setWorkingDir(NCommand::CFileSystem::getInstance().getCurrentPath());
        compiler->setWorkingDir("/home/dsadovyi/Coding");
        compiler->start();
        //compiler->wait();
        //mController->setSolutionPath(mFileSystem.getCurrentPath() + "/app");
    }
    else if(command == "cd")
    {
        NCommand::CFileSystem::getInstance().changeDir(*(args.begin()+1));
        emit endCommand();
    }
    else if(command == "exec")
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
        cmd->start();
    }
    else
    {
        emit error("Error. " + command + ": unknown command");
        handleEndCommand();
    }
}

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

bool CCommandHandler::sysCommand(const QString &command)
{
    static QSet <QString> availableCommands {"date", "time", "pwd", "ls" };
    return availableCommands.find(command) != availableCommands.end();
}


} // namespace NGraviz
