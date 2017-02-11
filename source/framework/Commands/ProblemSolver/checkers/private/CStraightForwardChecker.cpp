/**
 * Project   AlgoVi
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
    if(dataList == ansList)
    {
        if(dataList.size() == 1)
        {
            mCheckerMessage = *dataList.begin() + " == " + *ansList.begin();
        }
        else
        {
            mCheckerMessage = "Ok, Ok, results are equal";
        }
        return TCheckerResult::Success;
    }
    else
    {
        if(dataList.size() != ansList.size())
        {
            mCheckerMessage = "data.size() != ans.size() -> " +
                    QString::number(dataList.size()) + " != " + QString::number(ansList.size());
        }
        else
        {
            int i = 1;
            auto ansitr = ansList.begin();
            for(auto itr = dataList.begin(); itr != dataList.end(); ++itr, ++i, ++ansitr)
            {
                if(*itr != *ansitr)
                {
                    mCheckerMessage += "in " + QString::number(i) +
                            "-th value, expected = " + *ansitr + ", found " + *itr;
                    break;
                }
            }
        }
        return TCheckerResult::Fail;
    }
    return dataList == ansList ? TCheckerResult::Success : TCheckerResult::Fail;
}


} // namespace NCommand
