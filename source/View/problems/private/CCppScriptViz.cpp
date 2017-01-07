/**
 * Project   Graviz
 *
 * @file     CCppScriptViz.cpp
 * @author   Dmytro Sadovyi
 * @date     02.01.2017
 */

#include "../CCppScriptViz.h"
#include <QEventLoop>
#include <QMetaObject>

namespace NView
{

CCppScriptViz::CCppScriptViz(std::shared_ptr<NCommand::CCompilerHandler> compilerHandler, QString scriptPath)
    : mCompilerHandler(compilerHandler)
    , mScriptPath(scriptPath)
    , mProc(nullptr)
{
     if(!mCompilerHandler->isSourceCode(mScriptPath))
     {
         mCompilerHandler->addSourceCodePath(mScriptPath);
     }
}

QString CCppScriptViz::serialize(const std::map<QString, IGravizItem *> &items)
{
    mMutex.lock();
    qDebug () << "CCppScriptViz> serialize";

    mQueryHandler = new CQueryHandler;
    QThread* queryThread = new QThread;
    mQueryHandler->moveToThread(queryThread);
    queryThread->start();

    if(mCompilerHandler->isNeededCompilation(mScriptPath))
    {
        QEventLoop loop;
        connect(mCompilerHandler.get(), SIGNAL(finished(int)), &loop, SLOT(quit()));
        mCompilerHandler->performCompilation(mScriptPath, QStringList());
        loop.exec();
    }
    mBuffer.clear();
    mProc = new QProcess();
    connect(mProc, &QProcess::readyReadStandardOutput, [this, &items](){
        qDebug () << "mProc readyReadOutput";
        mQueryHandler->queryHandler(mProc->readAllStandardOutput(), items);
    });
    connect(mProc, &QProcess::readyReadStandardError, [this]{
        qDebug () << "mProc readyReadError";
        mBuffer += mProc->readAllStandardError();
    });
    connect(mQueryHandler, &CQueryHandler::data, [this](const QString& data){
        mProc->write(data.toLocal8Bit());
        mProc->write("\n");
    });
    mProc->start(mCompilerHandler->getAppPath(mScriptPath), QStringList() << "0");
    mProc->waitForFinished(10000000);
    qDebug () << "serialization finished: " << mBuffer;
    mProc->deleteLater();
    mMutex.unlock();
    return mBuffer;
}

void CCppScriptViz::realize(QString &data,
                           QPainter *painter,
                           const std::map <QString, IGravizItem*>& items)
{
    qDebug () << "CCppScriptViz> realize with data = " << data;
    if(mCompilerHandler->isNeededCompilation(mScriptPath))
    {
        QEventLoop loop;
        connect(mCompilerHandler.get(), SIGNAL(finished(int)), &loop, SLOT(quit()));
        mCompilerHandler->performCompilation(mScriptPath, QStringList());
        loop.exec();
    }
    mProc = new QProcess();
    connect(mProc, &QProcess::readyReadStandardOutput, [this, &items, painter](){
        mQueryHandler->queryHandler(mProc->readAllStandardOutput(), items, painter);
    });
    connect(mQueryHandler, &CQueryHandler::data, [this](const QString& data){
        mProc->write(data.toLocal8Bit());
        mProc->write("\n");
    });
    mProc->start(mCompilerHandler->getAppPath(mScriptPath), QStringList() << "1");
    mProc->write(data.toLocal8Bit());
    mProc->write("\n");
    mProc->waitForFinished(10000000);
    qDebug () << "realize finished";
    mProc = nullptr;
}

bool CCppScriptViz::isFree()
{
    return mProc == nullptr;
}

void CQueryHandler::queryHandler(QString query,
                                 const std::map<QString, IGravizItem *> &items,
                                 QPainter *painter)
{
    qDebug () << "queryHandler> with query: " << query;
    QTextStream stream(&query);
    QString cmd;
    while(!stream.atEnd())
    {
        stream >> cmd;
        if(cmd == "get")
        {
            QString toProc;
            QString what;
            stream >> what;
            qDebug () << "get " << what;
            if(items.find(what) == items.end())
                toProc = "null";
            else
                toProc = items.at(what)->serialize();
            qDebug () << "emit " << toProc;
            emit data(toProc);
        }
        else if(cmd == "draw")
        {
            QString what;
            stream >> what;
            if(what == "line")
            {
                double x[2], y[2];
                stream >> x[0] >> y[0];
                stream >> x[1] >> y[1];
                painter->drawLine(QPointF(x[0], y[0]), QPointF(x[1], y[1]));
            }
        }
        else if(cmd == "color")
        {
            int r, g, b;
            stream >> r >> g >> b;
            QPen p = painter->pen();
            p.setColor(QColor(r, g, b));
            painter->setPen(p);
        }
        else if(cmd == "width")
        {
            int val;
            stream >> val;
            QPen p = painter->pen();
            p.setWidth(val);
            painter->setPen(p);
        }
    }
}


} // namespace NView
