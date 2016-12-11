/**
 * Project   Graviz
 *
 * @file     CTestCommand.cpp
 * @author   Dmytro Sadovyi
 * @date     09.12.2016
 */

#include <QDebug>
#include <fstream>

#include "../CTestCommand.h"
#include "framework/Commands/CFileSystem.h"

namespace NCommand
{

CTestCommand::CTestCommand(const QStringList &args, std::shared_ptr<CTestProvider> testProvider)
    : ITerminalCommand(args)
    , mTestProvider(testProvider)
{
    mOptions.add_options()
        ("print,p", boost::program_options::value<std::string>(),
            "Print test\n"
            " [no arguments] -> print all tests\n"
            " -p 10 -> print 10-th test\n"
            " -p 1-3 -> print tests from 1 to 3")
        ("clear,c", boost::program_options::bool_switch()->default_value(false),
            "delete all tests from archive")
        ("delete,d", boost::program_options::value<int>(),
            "delete test by number")
        ("from-files", boost::program_options::value<std::string>(), "read tests from directory")
        ("to-files", boost::program_options::value<std::string>(), "write tests to files in directory[empty!]");
}

void CTestCommand::run()
{
    emit started();
    boost::program_options::variables_map vm;
    if(!readOptions(mArgs, vm))
    {
        emit finished(0);
        return;
    }

    if(vm.count("print"))
    {
        QString testNum = QString::fromStdString(vm["print"].as<std::string>());
        qDebug () << "print " << testNum;
        auto validateNum = [](const QString& str)
        {
            bool ret = true;
            for(char c : str.toStdString())
                ret &= isdigit(c);
            return ret;
        };
        if(testNum.indexOf("-") == -1)
        {
            if(validateNum(testNum))
            {
                int test = testNum.toInt();
                if(test < 1 || test > mTestProvider->size())
                {
                    QString toEmit = testNum + " - index out of range\n";
                    if(mTestProvider->size() == 0)
                        toEmit += "Test archive is empty\n";
                    else
                        toEmit += "Valid range is [1.." + QString::number(mTestProvider->size()) + "]\n";
                    emit error(toEmit);
                }
                else
                {
                    emit log(" * Test #" + QString::number(test) + "\n" + mTestProvider->getFormatted(test-1));
                }
            }
            else
            {
                emit error(testNum + " is not a valid number");
            }
        }
        else
        {
            QStringList range = testNum.split('-');
            if(range.size() != 2)
            {
                emit error(testNum + " is not valid number range");
            }
            else
            {
                QString strFrom = *range.begin();
                QString strTo = *(range.begin()+1);
                if(!validateNum(strFrom) || strFrom.toInt() < 1 || strFrom.toInt() > mTestProvider->size())
                {
                    emit error(strFrom + " is not valid number");
                }
                else if(!validateNum(strTo) || strTo.toInt() < 1 || strTo.toInt() > mTestProvider->size())
                {
                    emit error(strTo + " is not valid number");
                }
                else
                {
                    int from = strFrom.toInt();
                    int to = strTo.toInt();
                    if(from > to)
                    {
                        emit error(testNum + " - invalid range. " + strFrom + " > " + strTo);
                    }
                    else
                    {
                        for(size_t i = from-1; i < to; ++i)
                        {
                            QString toEmit = " * Test #" + QString::number(i+1) + "\n";
                            toEmit += mTestProvider->getFormatted(i);
                            emit log(toEmit);
                        }
                    }
                }
            }
        }

    }
    else if(vm.count("delete"))
    {
        qDebug () << "delete test";
        int testNum = vm["delete"].as<int>();
        if(testNum > 0 && testNum <= mTestProvider->size())
            mTestProvider->deleteTest(testNum-1);
        else
            emit error(QString::number(testNum) + " is not valid index");
    }
    else if(vm["clear"].as<bool>())
    {
        qDebug () << "clear test archive";
        mTestProvider->clear();
    }
    else if(vm.count("from-files"))
    {
        QString fullPath = CFileSystem::getInstance().getFullPath(
                    QString::fromStdString(vm["from-files"].as<std::string>())).c_str();
        if(!CFileSystem::getInstance().isDirectory(fullPath))
        {
            emit error(QString::fromStdString(vm["from-files"].as<std::string>()) + " is not a valid directory\n");
            emit finished(0);
            return;
        }
        int cnt = 0;
        for(int i = 1;;++i)
        {
            //QString fileData = QString("%1.dat").arg(i, 3, 10, QChar('0'));
            QString fileData = QString("%1").arg(i, 2, 10, QChar('0'));
            //QString fileAns = QString("%1.ans").arg(i, 3, 10, QChar('0'));
            QString fileAns = QString("%1.a").arg(i, 2, 10, QChar('0'));
            STest test;
            if(!CFileSystem::getInstance().isFile(fullPath+"/"+fileData)
                    || !CFileSystem::getInstance().isFile(fullPath + "/" + fileAns))
            {
                break;
            }
            ++cnt;
            std::ifstream file(fullPath.toStdString() + "/" + fileData.toStdString());
            std::string s;
            emit log("read file " + fileData + "\n");
            while(std::getline(file, s))
            {
                test.input += QString::fromStdString(s) + "\n";
            }
            file.close();
            file.open(fullPath.toStdString() + "/" + fileAns.toStdString());
            emit log("read file " + fileAns + "\n");
            while(std::getline(file, s))
            {
                test.output += QString::fromStdString(s) + "\n";
            }
            file.close();

            mTestProvider->addTest(test);
        }
        emit log(" [ Info ] " + QString::number(cnt) + " tests were read from files\n");
    }
    else if(vm.count("to-files"))
    {
        QString fullPath = CFileSystem::getInstance().getFullPath(
                    QString::fromStdString(vm["to-files"].as<std::string>())).c_str();
        if(!CFileSystem::getInstance().isDirectory(fullPath))
        {
            emit error(QString::fromStdString(vm["to-files"].as<std::string>()) + " is not a valid directory\n");
            emit finished(0);
            return;
        }
        int cnt = 0;
        for(int i = 0; i < mTestProvider->size(); ++i)
        {
            QString fileData = QString("%1.dat").arg(i+1, 3, 10, QChar('0'));
            QString fileAns = QString("%1.ans").arg(i+1, 3, 10, QChar('0'));
//            if(!CFileSystem::getInstance().isFile(fullPath+"/"+fileData)
//                    || !CFileSystem::getInstance().isFile(fullPath + "/" + fileAns))
//            {
//                break;
//            }
            ++cnt;
            std::ofstream file(fullPath.toStdString() + "/" + fileData.toStdString());
            emit log("write file " + fullPath + "/" + fileData + "\n");
            file << mTestProvider->get(i).input.toStdString();
            file.close();
            file.open(fullPath.toStdString() + "/" + fileAns.toStdString());
            emit log("write file " + fullPath + "/" + fileAns + "\n");
            file << mTestProvider->get(i).output.toStdString();
            file.close();
        }
        emit log(" [ Info ] " + QString::number(cnt) + " tests were written to files\n");
    }
    else
    {
        qDebug () << "print all tests";
        for(size_t i = 0; i < mTestProvider->size(); ++i)
        {
            QString toEmit = " * Test #" + QString::number(i+1) + "\n";
            toEmit += mTestProvider->getFormatted(i);
            emit log(toEmit);
        }
    }

    emit finished(0);
}

void CTestCommand::appendData(const QString &data)
{

}

void CTestCommand::terminate()
{

}


} // namespace NCommand
