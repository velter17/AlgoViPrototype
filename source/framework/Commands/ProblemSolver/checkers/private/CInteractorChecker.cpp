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
#include "../CInteractorChecker.h"

namespace NCommand
{

CInteractorChecker::CInteractorChecker(const QStringList &args, const QString &path)
    : IProblemChecker(args)
    , mPathToChecker(path)
{
    mOptions.add_options()
        ("input,i", boost::program_options::value<std::string>()->required()->default_value(""), "Input data")
        ("output,o", boost::program_options::value<std::string>()->required()->default_value(""), "Output data")
        ("answer,a", boost::program_options::value<std::string>()->required()->default_value(""), "Answer data");

    mCheckerMessage = "invalid checker";
}

TCheckerResult CInteractorChecker::check()
{
    if(!boost::filesystem::exists(boost::filesystem::path(mPathToChecker.toStdString())))
    {
        mCheckerMessage = mPathToChecker + " checker is invalid, see --help";
        return TCheckerResult::None;
    }
    QTemporaryDir dir;

    QProcess* proc = new QProcess();
    connect(proc, &QProcess::readyReadStandardOutput, [this, proc](){
        mCheckerMessage += proc->readAllStandardOutput();
    });
    connect(proc, &QProcess::readyReadStandardError, [this, proc]{
        mCheckerMessage += proc->readAllStandardError();
    });
    mCheckerMessage.clear();
    proc->start(mPathToChecker, QStringList() << QString::fromStdString(mVarMap["input"].as<std::string>())
                                               << QString::fromStdString(mVarMap["output"].as<std::string>())
                                               << QString::fromStdString(mVarMap["answer"].as<std::string>()));
    proc->waitForFinished(10000000);
    qDebug () << "interactor checker finished";
    TCheckerResult checker_result = proc->exitCode() == 0 ? TCheckerResult::Success : TCheckerResult::Fail;
//    std::string checker_result_msg;
//    std::ifstream res_file((dir.path() + "result.txt").toStdString());
//    mCheckerMessage.clear();
//    while(std::getline(res_file, checker_result_msg))
//    {
//        mCheckerMessage += QString::fromStdString(checker_result_msg) + " ";
//    }
//    if(!mCheckerMessage.isEmpty())
//    {
//        mCheckerMessage.chop(1);
//    }
    proc->deleteLater();
    return checker_result;
}


} // namespace NCommand
