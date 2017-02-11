/**
 * Project   AlgoVi
 *
 * @file     IProblemParser.cpp
 * @author   Dmytro Sadovyi
 * @date     10.12.2016
 */

#include "../IProblemParser.h"

namespace NCommand
{

IProblemParser::IProblemParser(const QStringList &args)
    : ITerminalCommand(args)
{
    mOptions.add_options()
        ("site,s", boost::program_options::value<std::string>()->required(),
         "url of problem to parse");
}

void IProblemParser::run()
{
    emit started();
    boost::program_options::variables_map vm;
    if(!readOptions(mArgs, vm))
    {
        emit finished(0);
        return;
    }
    emit finished(parse(QString::fromStdString(vm["site"].as<std::string>())));
}

void IProblemParser::appendData(const QString &data)
{

}

void IProblemParser::terminate()
{

}

const std::vector<STest> IProblemParser::tests() const
{
    return mTests;
}


} // namespace NCommand
