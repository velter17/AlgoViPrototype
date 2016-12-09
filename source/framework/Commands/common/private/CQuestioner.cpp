/**
 * Project   Graviz
 *
 * @file     CQuestioner.cpp
 * @author   Dmytro Sadovyi
 * @date     09.12.2016
 */

#include "../CQuestioner.h"

namespace NCommand
{

CQuestioner::CQuestioner(const QStringList &args)
   : ITerminalCommand(args)
{
   mOptions.add_options()
      ("question,q", boost::program_options::value<std::string>(&mQuestion)->required(), "Question string")
      ("option,o", boost::program_options::value<std::vector<std::string>>(&mOptionsVector)->multitoken(), "answer option for question");
}

void CQuestioner::run()
{
   emit started();
   boost::program_options::variables_map vm;
   if(!readOptions(mArgs, vm) || mOptionsVector.empty())
   {
      emit finished(0);
      return;
   }

   QString msgToEmit = "\n: " + QString::fromStdString(mQuestion) + "\n";
   for(const std::string& opt : mOptionsVector)
   {
      msgToEmit += " " + QString::fromStdString(opt) + "\n";
   }
   emit log(msgToEmit);
}

void CQuestioner::appendData(const QString &data)
{
   emit finished(static_cast<int>(data.toStdString()[0]));
}

void CQuestioner::terminate()
{
   appendData("~");
}


} // namespace NCommand
