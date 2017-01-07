/**
 * Project   Graviz
 *
 * @file     CVisualSolver.cpp
 * @author   Dmytro Sadovyi
 * @date     16.12.2016
 */

#include "../CVisualSolver.h"

namespace NCommand
{

CVisualSolver::CVisualSolver(const QStringList &args, std::shared_ptr<CCompilerHandler> compilerHandler)
    : ITerminalCommand(args)
    , mCompilerHandler(compilerHandler)
    , mInputTimestamp(0)
    , mLastTimestamp(0)
    , mCompiledFlag(true)
    , mSolvedFlag(true)
{
    mOptions.add_options()
        ("src,s", boost::program_options::value<std::string>()->required(), "source code")
        ("algo,a", boost::program_options::value<std::string>()->required(), "algo cpp script")
        ("flag,f", boost::program_options::value<std::vector<std::string>>(&mFlagParsed),
            "compilation flags\n"
            "use c++ -DVAL like -f DVAL");
}


bool CVisualSolver::init()
{
    if(readOptions(mArgs, mVarMap))
    {
        for(const std::string& f : mFlagParsed)
        {
            mFlags << "-f" << QString::fromStdString(f);
        }
        mSourceCode = CFileSystem::getInstance().getFullPath(
                            QString::fromStdString(mVarMap["src"].as<std::string>())).c_str();
//        mAlgoScript = CFileSystem::getInstance().getFullPath(
//                            QString::fromStdString(mVarMap["algo"].as<std::string>())).c_str();
        mAlgoScript = QString::fromStdString(mVarMap["algo"].as<std::string>());
        return true;
    }
    else
    {
        return false;
    }
}

void CVisualSolver::run()
{
    qDebug () << "CVisualSolver> run";
    emit started();
    if(!mCompilerHandler->isSourceCode(mSourceCode))
    {
        mCompilerHandler->addSourceCodePath(mSourceCode);
    }
    mSolver.reset(new CProblemSolver(QStringList() <<
                           "--src" << mCompilerHandler->getAppPath(mSourceCode), nullptr));
    connect(mSolver.get(), &CProblemSolver::log, [this](const QString& msg){
        //emit log(msg);
        mOutput += msg;
    });
    connect(mSolver.get(), &CProblemSolver::error, [this](const QString& msg){
        emit error(msg);
    });
    connect(mSolver.get(), &CProblemSolver::finished, [this](int code){
        qDebug () << "CVisualSolver> mSolver finished";
        if(code == 0)
        {
            emit log(mOutput);
            mOutput.clear();
            mSolvedFlag = true;
        }
        if(mLastTimestamp != mInputTimestamp)
        {
            startSolver();
        }
    });
    if(!mSolver->init())
    {
        emit finished(1);
    }
    mSolver->setAppPath(mCompilerHandler->getAppPath(mSourceCode));
}

void CVisualSolver::terminate()
{
    emit finished(0);
}

void CVisualSolver::appendData(const QString &data)
{
    emit command(data);
}

void CVisualSolver::setInput(const QString &data)
{
    mInput = data;
    mInputTimestamp++;
    if(mSolvedFlag && mCompiledFlag && !mInput.isEmpty())
    {
        startSolver();
    }
}

QString CVisualSolver::getAlgoScriptPath()
{
    return mAlgoScript;
}

void CVisualSolver::startSolver()
{
    mCompiledFlag = false;
    qDebug () << "CVisualSolver> startSolver";
    if(mCompilerHandler->isNeededCompilation(mSourceCode))
    {
        qDebug () << "neededCompilation";
        std::shared_ptr<QMetaObject::Connection> pconn(new QMetaObject::Connection);
        QMetaObject::Connection &conn = *pconn;
        conn = connect(mCompilerHandler.get(), &CCompilerHandler::finished,
                       [this, pconn, &conn](int code){
            qDebug () << "compilation was finished";
            if(code == 0)
            {
                runSolver();
            }
            else
            {
                emit error(" [ Error ] problem with compilation. Exit.\n");
                mInputTimestamp = -1;
                emit finished(1);
            }
            mCompiledFlag = true;
            disconnect(conn);
        });
        qDebug () << "Run compilation";

        mCompilerHandler->performCompilation(mSourceCode, QStringList());
    }
    else
    {
        mCompiledFlag = true;
        qDebug () << "no need compilation";
        runSolver();
    }

    qDebug () << "CVisualSolver> end of solver";
}

void CVisualSolver::runSolver()
{
    qDebug () << "CVisualSolver> runSolver";
    mLastTimestamp = mInputTimestamp;
    mSolvedFlag = false;
    mSolver->run();
    qDebug () << "CVisualSolver> mSolver> run";
    mSolver->appendData(mInput);
}


} // namespace NCommand
