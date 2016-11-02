#include "framework/Commands/system/CSystemCmd.h"
#include <QDebug>

CSystemCmd::CSystemCmd(QStringList args)
    : ITerminalCommand(args)
{

}

void CSystemCmd::run()
{
    QProcess proc;
    connect(&proc, &QProcess::readyReadStandardError, [this, &proc](){
        emit error(proc.readAllStandardError());
    });
    connect(&proc, &QProcess::readyReadStandardOutput, [this, &proc](){
        emit log(proc.readAllStandardOutput());
    });
    //proc->setStandardErrorFile("/home/dmitriy/lol/log.txt");
    qDebug () << mDirectory;
    proc.setWorkingDirectory(mDirectory);
    proc.start("bash", QStringList() << "-c" << mArgs);
    proc.waitForFinished(2*60*60*10);
}

