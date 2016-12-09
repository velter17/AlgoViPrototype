/**
 * Project   Graviz
 *
 * @file     CTestCommand.cpp
 * @author   Dmytro Sadovyi
 * @date     09.12.2016
 */

#include <QDebug>

#include "../CTestCommand.h"

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
            "delete test by number");
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
