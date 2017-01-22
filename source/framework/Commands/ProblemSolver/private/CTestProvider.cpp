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
    static const int maxStrLen = 200;

   QVector<QString> inputList;
   if(mTests[i].input.length() > maxStrLen)
   {
       for(const QString& str : (mTests[i].input.mid(0, maxStrLen)+" ...").split('\n', QString::SkipEmptyParts))
          inputList.append(str);
   }
   else
   {
       for(const QString& str : mTests[i].input.split('\n', QString::SkipEmptyParts))
          inputList.append(str);
   }
   QVector<QString> outputList;
   if(mTests[i].output.length() > maxStrLen)
   {
       for(const QString& str : (mTests[i].output.mid(0, maxStrLen)+" ..").split('\n', QString::SkipEmptyParts))
          outputList.append(str);
   }
   else
   {
       for(const QString& str : mTests[i].output.split('\n', QString::SkipEmptyParts))
          outputList.append(str);
   }
   qDebug () << "input : " << inputList;
   qDebug () << "output : " << outputList;
   int maxLen[2] = {5, 6};
   for(const QString& str : inputList)
      maxLen[0] = std::max(maxLen[0], str.length());
   for(const QString& str : outputList)
      maxLen[1] = std::max(maxLen[1], str.length());
   if(maxLen[0] > 50 || maxLen[1] > 50 || inputList.size() > 50 || outputList.size() > 50)
   {
       return " Not small enough to display :(\n";
   }

   const QString borderColor = "#A0A0A0";
   const QString inputTitleColor = "#BFA8FF";
   const QString outputTitleColor = "#FF9999";

   QString ret = "&nbsp;<font color=" + borderColor + ">&#x250c;";
   ret += QString("&#x2500;").repeated(maxLen[0]+2);
   //ret.append('+');
   ret += "&#x252c;";
   ret += QString("&#x2500;").repeated(maxLen[1]+2);
   ret += "&#x2510;<br>&nbsp;&#x2502;";
   ret += QString("&nbsp;").repeated((maxLen[0]-3) / 2);
   ret += "</font><font color=" + inputTitleColor + ">input</font><font color=" + borderColor + ">";
   ret += QString("&nbsp;").repeated(maxLen[0]+2 - (maxLen[0]-3) / 2 - 5);
   //ret.append('|');
   ret += "&#x2502;";
   ret += QString("&nbsp;").repeated((maxLen[1]-4) / 2);
   ret += "</font><font color=" + outputTitleColor + ">output</font><font color=" + borderColor + ">";
   ret += QString("&nbsp;").repeated(maxLen[1]+2 - (maxLen[1]-4) / 2 - 6);
   ret += "&#x2502;<br>&nbsp;";
   //ret.append('+');
   ret += "&#x251c;";
   ret += QString("&#x2500;").repeated(maxLen[0]+2);
   //ret.append('+');
   ret += "&#x253c;";
   ret += QString("&#x2500;").repeated(maxLen[1]+2);
   ret += "&#x2524;<br>&nbsp;";
   int to = std::max(inputList.size(), outputList.size());
   for(int i = 0; i < to; ++i)
   {
      //ret.append('|');
       ret += "&#x2502;";
      if(inputList.size() <= i)
      {
         ret += QString("&nbsp;").repeated(maxLen[0]+2);
      }
      else
      {
         ret += "&nbsp;</font>";
         ret += inputList[i];
         ret += "<font color=" + borderColor + ">";
         ret += QString("&nbsp;").repeated(maxLen[0] - inputList[i].length() + 1);
      }
      //ret.append('|');
      ret += "&#x2502;";
      if(outputList.size() <= i)
      {
         ret += QString("&nbsp;").repeated(maxLen[1]+2);
         ret += "&#x2502;<br>&nbsp;";
         continue;
      }
      else
      {
          ret += "&nbsp;</font>";
          ret += outputList[i];
          ret += "<font color=" + borderColor + ">";
          ret += QString("&nbsp;").repeated(maxLen[1] - outputList[i].length() + 1);
          ret += "&#x2502;<br>&nbsp;";
      }
   }
   //ret.append('+');
   ret += "&#x2514;";
   ret += QString("&#x2500;").repeated(maxLen[0]+2);
   //ret.append('+');
   ret += "&#x2534;";
   ret += QString("&#x2500;").repeated(maxLen[1]+2);
   ret += "&#x2518;<br></font>";
   return ret;
}


} // namespace NCommand
