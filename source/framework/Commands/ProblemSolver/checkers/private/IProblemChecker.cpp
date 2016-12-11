/**
 * Project   Graviz
 *
 * @file     IProblemChecker.cpp
 * @author   Dmytro Sadovyi
 * @date     10.12.2016
 */

#include "../IProblemChecker.h"

namespace NCommand
{

IProblemChecker::IProblemChecker(const QStringList &args)
    : ITerminalCommand(args)
{

}

void IProblemChecker::run()
{
    emit started();
    if(!readOptions(mArgs, mVarMap))
    {
        emit finished(0);
        return;
    }
    mResultCode = check();
    emit finished(0);
}

void IProblemChecker::appendData(const QString &data)
{

}

void IProblemChecker::terminate()
{

}

QString IProblemChecker::details()
{
    return mCheckerMessage;
}

TCheckerResult IProblemChecker::getResult()
{
    return mResultCode;
}


} // namespace NCommand
