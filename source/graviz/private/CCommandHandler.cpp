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

namespace NGraviz
{

CCommandHandler::CCommandHandler(CGravizSystem *parent)
    : mParent(parent)
{

}

void CCommandHandler::handle(const QString& commandStr)
{
    qDebug () << "CCommandHandler> handle " << commandStr;
    QStringList args = commandStr.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    QString command = *args.begin();
    if(command == "compile")
    {
        args.erase(args.begin());
        NCommand::CCompiler* compiler = new NCommand::CCompiler(args);
        connect(compiler, SIGNAL(finished()), compiler, SLOT(deleteLater()));
        connect(compiler, SIGNAL(error(QString)), this, SLOT(handleError(QString)));
        connect(compiler, SIGNAL(log(QString)), this, SLOT(handleLog(QString)));
        connect(compiler, SIGNAL(logHtml(QString)), this, SLOT(handleHtml(QString)));
        connect(compiler, SIGNAL(finished()), this, SLOT(handleEndCommand()));
        compiler->setWorkingDir(NCommand::CFileSystem::getInstance().getCurrentPath());
        compiler->start();
        //mController->setSolutionPath(mFileSystem.getCurrentPath() + "/app");
    }
    else if(command == "cd")
    {
        NCommand::CFileSystem::getInstance().changeDir(*(args.begin()+1));
        emit endCommand();
    }
    else
    {
        NCommand::CSystemCmd* cmd = new NCommand::CSystemCmd(QStringList() << commandStr);
        connect(cmd, SIGNAL(finished()), cmd, SLOT(deleteLater()));
        connect(cmd, SIGNAL(error(QString)), this, SLOT(handleError(QString)));
        connect(cmd, SIGNAL(log(QString)), this, SLOT(handleLog(QString)));
        connect(cmd, SIGNAL(finished()), this, SLOT(handleEndCommand()));
        cmd->setWorkingDir(NCommand::CFileSystem::getInstance().getCurrentPath());
        cmd->start();
    }
    //else if(!cmddata.title.isEmpty())
    //    emit error("Error. " + cmddata.title + ": unknown command");
}

void CCommandHandler::handleLog(QString msg)
{
    emit log(msg);
}

void CCommandHandler::handleError(QString msg)
{
    emit error(msg);
}

void CCommandHandler::handleHtml(QString msg)
{
    emit logHtml(msg);
}

void CCommandHandler::handleEndCommand()
{
    emit endCommand();
}


} // namespace NGraviz
