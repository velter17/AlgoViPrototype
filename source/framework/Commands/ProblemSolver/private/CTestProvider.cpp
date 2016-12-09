/**
 * Project   Graviz
 *
 * @file     CTestProvider.cpp
 * @author   Dmytro Sadovyi
 * @date     09.12.2016
 */

#include <cassert>
#include <QVector>
#include <QStringList>
#include <QDebug>

#include "../CTestProvider.h"

namespace NCommand
{

CTestProvider::CTestProvider()
{

}

void CTestProvider::addTest(const STest &test)
{
   mTests.push_back(test);
}

void CTestProvider::clear()
{
   mTests.clear();
}

size_t CTestProvider::size() const
{
    return mTests.size();
}

void CTestProvider::deleteTest(int i)
{
    qDebug () << "CTestProvider> deleteTest " << i;
    assert(i >= 0 && i < mTests.size());
    for(int j = i+1; j < mTests.size(); ++j)
        std::swap(mTests[j], mTests[j-1]);
    mTests.pop_back();
}

STest CTestProvider::get(int i) const
{
   assert(i >= 0 && i < mTests.size());
   return mTests[i];
}

QString CTestProvider::getFormatted(int i) const
{
   QVector<QString> inputList;
   for(const QString& str : mTests[i].input.split('\n', QString::SkipEmptyParts))
      inputList.append(str);
   QVector<QString> outputList;
   for(const QString& str : mTests[i].output.split('\n', QString::SkipEmptyParts))
      outputList.append(str);
   qDebug () << "input : " << inputList;
   qDebug () << "output : " << outputList;
   int maxLen[2] = {5, 6};
   for(const QString& str : inputList)
      maxLen[0] = std::max(maxLen[0], str.length());
   for(const QString& str : outputList)
      maxLen[1] = std::max(maxLen[1], str.length());
   QString ret = "+";
   for(int i = 0; i < maxLen[0]+2; ++i)
      ret.append('-');
   ret.append('+');
   for(int i = 0; i < maxLen[1]+2; ++i)
      ret.append('-');
   ret.append('+');
   ret.append('\n');
   ret.append('|');
   for(int i = 0; i < (maxLen[0]-3) / 2; ++i)
      ret.append(' ');
   ret += "input";
   for(int i = 0; i < maxLen[0]+2 - (maxLen[0]-3) / 2 - 5; ++i)
      ret.append(' ');
   ret.append('|');
   for(int i = 0; i < (maxLen[1]-4) / 2; ++i)
      ret.append(' ');
   ret += "output";
   for(int i = 0; i < maxLen[1]+2 - (maxLen[1]-4) / 2 - 6; ++i)
      ret.append(' ');
   ret += "|\n";
   ret.append('+');
   for(int i = 0; i < maxLen[0]+2; ++i)
      ret.append('-');
   ret.append('+');
   for(int i = 0; i < maxLen[1]+2; ++i)
      ret.append('-');
   ret.append('+');
   ret.append('\n');
   int to = std::max(inputList.size(), outputList.size());
   for(int i = 0; i < to; ++i)
   {
      ret.append('|');
      if(inputList.size() <= i)
      {
         for(int j = 0; j < maxLen[0]+2; ++j)
            ret.append(' ');
      }
      else
      {
         ret.append(' ');
         ret += inputList[i];
         for(int j = 0; j < maxLen[0] - inputList[i].length() + 1; ++j)
            ret.append(' ');
      }
      ret.append('|');
      if(outputList.size() <= i)
      {
         for(int j = 0; j < maxLen[1]+2; ++j)
            ret.append(' ');
         ret += "|\n";
         continue;
      }
      else
      {
         ret.append(' ');
         ret += outputList[i];
         for(int j = 0; j < maxLen[1] - outputList[i].length() + 1; ++j)
            ret.append(' ');
         ret += "|\n";
      }
   }
   ret.append('+');
   for(int i = 0; i < maxLen[0]+2; ++i)
      ret.append('-');
   ret.append('+');
   for(int i = 0; i < maxLen[1]+2; ++i)
      ret.append('-');
   ret.append('+');
   ret.append('\n');
   return ret;
}


} // namespace NCommand
