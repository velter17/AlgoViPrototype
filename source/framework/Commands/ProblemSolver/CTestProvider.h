/**
 * Project   AlgoVi
 *
 * @file     CTestProvider.h
 * @author   Dmytro Sadovyi
 * @date     09.12.2016
 */

#pragma once

#include <QString>

namespace NCommand
{

struct STest
{
   QString input;
   QString output;
   STest(QString in = "", QString out = "")
      : input(in)
      , output(out)
   {}
};

class CTestProvider
{
public: // methods
    CTestProvider();

    void addTest(const STest& test);
    void clear();
    size_t size() const;

    void deleteTest(int i);
    void swapTests(int a, int b);

    STest get(int i) const;
    QString getFormatted(int i) const;
    void setTest(int i, const STest& test);
private:
    std::vector<STest> mTests;
};

} // namespace NCommand

