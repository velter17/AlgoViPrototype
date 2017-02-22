/**
 * Project   AlgoVi
 *
 * @file     CTestValidator.cpp
 * @author   Dmytro Sadovyi
 * @date     21.02.2017
 */

#include <QTemporaryDir>
#include <QProcess>
#include <fstream>

#include "framework/Commands/CFileSystem.h"
#include "../CTestValidator.h"

namespace NCommand
{

CTestValidator::CTestValidator(const QStringList &args, const QString &appPath)
    : IProblemChecker(args)
    , mPathToValidator(appPath)
{
    mOptions.add_options()
        ("input,i", boost::program_options::value<std::string>()->required()->default_value(""), "Input data")
        ("output,o", boost::program_options::value<std::string>()->required()->default_value(""), "Output data");
    qDebug () << "CTestValidator():" << appPath;
}

TCheckerResult CTestValidator::check()
{
    qDebug () << "CTestValidator> check";
    QTemporaryDir dir;
    std::ofstream file((dir.path() + "input.txt").toStdString());
    file << mVarMap["input"].as<std::string>();
    file.close();

    file.open((dir.path() + "output.txt").toStdString());
    file << mVarMap["output"].as<std::string>();
    file.close();

    QProcess* proc = new QProcess();
    connect(proc, &QProcess::readyReadStandardOutput, [this, proc](){
        mCheckerMessage += proc->readAllStandardOutput();
    });
    connect(proc, &QProcess::readyReadStandardError, [this, proc]{
        mCheckerMessage += proc->readAllStandardError();
    });
    qDebug () << "run validator at path " << mPathToValidator;
    proc->start(mPathToValidator, QStringList() << (dir.path() + "input.txt")
                                                << (dir.path() + "output.txt"));
    proc->waitForFinished(10000000);
    qDebug () << "validator finished";
    TCheckerResult validation_status = proc->exitCode() == 0 ? TCheckerResult::Success : TCheckerResult::Fail;
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
    return validation_status;
}


} // namespace NCommand
