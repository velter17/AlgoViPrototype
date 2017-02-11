/**
 * Project   AlgoVi
 *
 * @file     CTestLibChecker.cpp
 * @author   Dmytro Sadovyi
 * @date     09.01.2017
 */

#include <QTemporaryDir>
#include <QProcess>
#include <fstream>

#include "framework/Commands/CFileSystem.h"
#include "../CTestLibChecker.h"

namespace NCommand
{

CTestLibChecker::CTestLibChecker(const QStringList &args, const QString &type)
    : IProblemChecker(args)
{
    mOptions.add_options()
        ("input,i", boost::program_options::value<std::string>()->required()->default_value(""), "Input data")
        ("output,o", boost::program_options::value<std::string>()->required()->default_value(""), "Output data")
        ("answer,a", boost::program_options::value<std::string>()->required()->default_value(""), "Answer data");

    mCheckerMessage = "invalid checker";
    mPathToChecker = "checkers/" + type;
}

TCheckerResult CTestLibChecker::check()
{
    if(!boost::filesystem::exists(boost::filesystem::path(mPathToChecker.toStdString())))
    {
        mCheckerMessage = mPathToChecker.mid(9) + " checker is invalid, see --help";
        return TCheckerResult::None;
    }
    QTemporaryDir dir;
    std::ofstream file((dir.path() + "input.txt").toStdString());
    file << mVarMap["input"].as<std::string>();
    file.close();

    file.open((dir.path() + "output.txt").toStdString());
    file << mVarMap["output"].as<std::string>();
    file.close();

    file.open((dir.path() + "answer.txt").toStdString());
    file << mVarMap["answer"].as<std::string>();
    file.close();

    QProcess* proc = new QProcess();
    connect(proc, &QProcess::readyReadStandardOutput, [this, proc](){
        mCheckerMessage += proc->readAllStandardOutput();
    });
    connect(proc, &QProcess::readyReadStandardError, [this, proc]{
        mCheckerMessage += proc->readAllStandardError();
    });
    proc->start(mPathToChecker, QStringList() << (dir.path() + "input.txt")
                                               << (dir.path() + "output.txt")
                                               << (dir.path() + "answer.txt")
                                               << (dir.path() + "result.txt"));
    proc->waitForFinished(10000000);
    qDebug () << "testlib checker finished";
    TCheckerResult checker_result = proc->exitCode() == 0 ? TCheckerResult::Success : TCheckerResult::Fail;
    std::string checker_result_msg;
    std::ifstream res_file((dir.path() + "result.txt").toStdString());
    mCheckerMessage.clear();
    while(std::getline(res_file, checker_result_msg))
    {
        mCheckerMessage += QString::fromStdString(checker_result_msg) + " ";
    }
    if(!mCheckerMessage.isEmpty())
    {
        mCheckerMessage.chop(1);
    }
    proc->deleteLater();
    return checker_result;
}


} // namespace NCommand
