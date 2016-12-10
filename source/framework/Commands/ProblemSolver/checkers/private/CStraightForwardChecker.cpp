/**
 * Project   Graviz
 *
 * @file     CStraightForwardChecker.cpp
 * @author   Dmytro Sadovyi
 * @date     10.12.2016
 */

#include "../CStraightForwardChecker.h"

namespace NCommand
{

CStraightForwardChecker::CStraightForwardChecker(const QStringList &args)
    : IProblemChecker(args)
{
    mOptions.add_options()
        ("data", boost::program_options::value<std::string>()->required(), "Data from application")
        ("answer", boost::program_options::value<std::string>()->required(), "Expected answer");
    mOptionsPositional.add("data", 1);
    mOptionsPositional.add("answer", 2);
}

TCheckerResult CStraightForwardChecker::check()
{
    QString data = QString::fromStdString(mVarMap["data"].as<std::string>());
    QString ans = QString::fromStdString(mVarMap["answer"].as<std::string>());
    QStringList dataList = data.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    QStringList ansList = ans.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    qDebug () << "dataList : " << dataList;
    qDebug () << "ansList : " << ansList;
    return dataList == ansList ? TCheckerResult::Success : TCheckerResult::Fail;
}


} // namespace NCommand
