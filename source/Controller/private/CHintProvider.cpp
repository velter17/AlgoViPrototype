/**
 * Project   Graviz
 *
 * @file     CHintProvider.cpp
 * @author   Dmytro Sadovyi
 * @date     15.12.2016
 */

#include <QDebug>

#include "framework/Commands/ProblemSolver/CTestCommand.h"
#include "framework/Commands/ProblemSolver/CProblemSolver.h"
#include "framework/Commands/system/CCompiler.h"
#include "framework/Commands/ProblemSolver/CProblemTester.h"
#include "framework/Commands/CFileSystem.h"
#include "../CHintProvider.h"

namespace NController
{

template <>
tHint CHintProvider::hints<TCommandHintName::SystemPath>(const QString& input)
{
    return NCommand::CFileSystem::getInstance().getHint(input);
}

template <>
tHint CHintProvider::hints<TCommandHintName::RunSolver>(const QString& input)
{
    NCommand::CProblemSolver solver(QStringList(), nullptr);
    return tHint(solver.getOptionList(), 0);
}

template <>
tHint CHintProvider::hints<TCommandHintName::Test>(const QString& input)
{
    NCommand::CTestCommand test(QStringList(), nullptr, nullptr);
    return tHint(test.getOptionList(), 0);
}

template <>
tHint CHintProvider::hints<TCommandHintName::Compile>(const QString& input)
{
//    NCommand::CCompiler compiler(QStringList());
//    return tHint(compiler.getOptionList(), 0);
    return tHint(QStringList(), 0);
}

template <>
tHint CHintProvider::hints<TCommandHintName::Tester>(const QString& input)
{
    NCommand::CTestCommand tester(QStringList(), nullptr, nullptr);
    return tHint(tester.getOptionList(), 0);
}

template <>
tHint CHintProvider::hints<TCommandHintName::AppName>(const QString& input)
{
    return tHint(mAppList, 0);
}

CHintProvider::CHintProvider()
{
    mFunc["run"] = &CHintProvider::hints<TCommandHintName::RunSolver>;
    mFunc["test"] = &CHintProvider::hints<TCommandHintName::Test>;
    mFunc["tester"] = &CHintProvider::hints<TCommandHintName::Tester>;
    mFunc["compile"] = &CHintProvider::hints<TCommandHintName::Compile>;

    // internal commands
    mAppList << "run" << "test" << "tester" << "compile"

    // external commands
             << "ls" << "date" << "time" << "cd" << "pwd"
             << "exit";
}

tHint CHintProvider::getHints(const QString& input)
{
    qDebug () << "CHintProvider> handle " << input;
    QStringList args = input.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    if(args.size() == 0)
        return tHint(QStringList(), 0);
    if(args.size() == 1)
        return processHints(hints<TCommandHintName::AppName>(input).first, args.first());
    else
    {
        QString last = args.last();
        QString cmd = *args.begin();
        if(mFunc.find(cmd) != mFunc.end() && last.length() >= 2 && last.mid(0, 2) == "--")
            return processHints((this->*mFunc[cmd])(input).first, last.mid(2, last.length()-2));
        else
            return hints<TCommandHintName::SystemPath>(last);
    }
}

tHint CHintProvider::processHints(const QStringList &list, const QString &str)
{
    tHint ret;
    ret.second = str.length();
    for(const QString s : list)
    {
        if(s.length() >= str.length() && s.mid(0, str.length()) == str)
            ret.first << s;
    }
    if(ret.first.isEmpty())
        ret.second = 0;
    return ret;
}

} // namespace NController
