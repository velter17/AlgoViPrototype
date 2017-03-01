/**
 * Project   AlgoVi
 *
 * @file     CVisualViewer.cpp
 * @author   Dmytro Sadovyi
 * @date     27.02.2017
 */

#include "../CVisualViewer.h"

namespace NCommand
{

CVisualViewer::CVisualViewer(const QStringList &args, std::shared_ptr<CCompilerHandler> compilerHandler)
    : ITerminalCommand(args)
{
    mOptions.add_options()
        ("viewer,v", boost::program_options::value<std::string>()->required(), "source code");
}


bool CVisualViewer::init()
{
    if(readOptions(mArgs, mVarMap))
    {
//        for(const std::string& f : mFlagParsed)
//        {
//            mFlags << "-f" << QString::fromStdString(f);
//        }
        mViewerScriptPath = QString::fromStdString(mVarMap["viewer"].as<std::string>());
        return true;
    }
    else
    {
        return false;
    }
}

void CVisualViewer::run()
{
    qDebug () << "CVisualViewer> run";
    emit started();
}

void CVisualViewer::terminate()
{
    emit finished(0);
}

void CVisualViewer::appendData(const QString &data)
{
    emit command(data);
}

QString CVisualViewer::getViewerScriptPath()
{
    return mViewerScriptPath;
}


} // namespace NCommand
