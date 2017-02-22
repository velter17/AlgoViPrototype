/**
 * Project   AlgoVi
 *
 * @file     CCustomChecker.cpp
 * @author   Dmytro Sadovyi
 * @date     22.02.2017
 */

#include <QTemporaryDir>
#include <QProcess>
#include <fstream>

#include "framework/Commands/CFileSystem.h"
#include "../CCustomChecker.h"

namespace NCommand
{

CCustomChecker::CCustomChecker(const QStringList &args, const QString &path)
    : IProblemChecker(args)
    , mPathToChecker(path)
{
    mOptions.add_options()
        ("input,i", boost::program_options::value<std::string>()->required()->default_value(""), "Input data")
        ("output,o", boost::program_options::value<std::string>()->required()->default_value(""), "Output data")
        ("answer,a", boost::program_options::value<std::string>()->required()->default_value(""), "Answer data");

    mCheckerMessage = "invalid checker";
}

TCheckerResult CCustomChecker::check()
{
    if(!boost::filesystem::exists(boost::filesystem::path(mPathToChecker.toStdString())))
    {
        mCheckerMessage = mPathToChecker + " checker is invalid, see --help";
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
    qDebug () << "custom checker finished";
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
