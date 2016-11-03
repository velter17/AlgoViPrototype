/**
 * Project   Graviz
 *
 * @file     CTerminal.cpp
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#include <QTextBlock>
#include <QApplication>
#include <iostream>
#include <QDebug>
#include <QTimer>

#include "Controller/CTerminal.h"
#include "framework/Commands/system/CSystemCmd.h"
#include "framework/Commands/CFileSystem.h"
#include "framework/Commands/system/CCompiler.h"

namespace NController
{

QColor CTerminal::Colors::Background = QColor(0,0,0);
QColor CTerminal::Colors::Main = QColor(0,255,0);
QColor CTerminal::Colors::Error = QColor(255,20,100);
QColor CTerminal::Colors::Output = QColor(255,255,255);

CTerminal::CTerminal(QWidget *parent)
    : QPlainTextEdit(parent)
    , mPromptString("graviz")
    , mLocked(false)
    , mTabPressCount(0)
{
    mHistoryItr = mHistory.end();

    mPalette.setColor(QPalette::Base, Colors::Background);
    mPalette.setColor(QPalette::Text, Colors::Main);
    setPalette(mPalette);

    connect(&mFileSystem, SIGNAL(error(QString)), this, SLOT(appendError(QString)));

    newCmdPrompt();
}

void CTerminal::keyPressEvent(QKeyEvent *e)
{
    if(mLocked)
        return;

    /* Single character */
    if(e->key() >= 0x20 && e->key() <= 0x7e
           && (e->modifiers() == Qt::NoModifier || e->modifiers() == Qt::ShiftModifier))
                QPlainTextEdit::keyPressEvent(e);

    /* Delete one character by backspace key */
    if(e->key() == Qt::Key_Backspace
           && e->modifiers() == Qt::NoModifier
           && textCursor().positionInBlock() > mPromptStringLen)
            QPlainTextEdit::keyPressEvent(e);

    /* Restore command, which was been typed before */
    if(e->key() == Qt::Key_Up)
    {
        qDebug () << mHistory;
        if(mHistoryItr != mHistory.begin() && !mHistory.isEmpty())
            --mHistoryItr;
        if(mHistoryItr != mHistory.end())
        {
            QTextBlock block = document()->lastBlock();
            QTextCursor p(block);
            p.select(QTextCursor::BlockUnderCursor);
            p.removeSelectedText();
            newCmdPrompt();
            p.movePosition(QTextCursor::EndOfBlock);
            p.insertText(*mHistoryItr);
        }
    }

    /* Restore command, which was been typed after */
    if(e->key() == Qt::Key_Down)
    {
        if(mHistoryItr != mHistory.end() && !mHistory.isEmpty())
            ++mHistoryItr;
        if(mHistoryItr != mHistory.end())
        {
            QTextBlock block = document()->lastBlock();
            QTextCursor p(block);
            p.select(QTextCursor::BlockUnderCursor);
            p.removeSelectedText();
            /*p.movePosition(QTextCursor::StartOfBlock);
            p.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
            p.removeSelectedText();*/
            newCmdPrompt();
            p.movePosition(QTextCursor::EndOfBlock);
            p.insertText(*mHistoryItr);
        }
    }

    /* Move cursor left or right */
    if(e->key() == Qt::Key_Left || e->key() == Qt::Key_Right)
    {
        if(!(e->key() == Qt::Key_Left && textCursor().columnNumber() == mPromptStringLen))
        {
            QTextCursor cursor = textCursor();
            cursor.movePosition(e->key() == Qt::Key_Left ? QTextCursor::Left : QTextCursor::Right);
            this->setTextCursor(cursor);
        }
    }

    /* Query for typing hint
     * One tab - autocompletion of current word
     * Two tabs - list of completion hints
    */
    if(e->key() == Qt::Key_Tab)
    {
        if(mTabPressCount == 0)
        {
            QTimer::singleShot(200, this, SLOT(tabKeyHandler()));
        }
        ++mTabPressCount;
    }

    /* Execute command */
    if(e->key() == Qt::Key_Return && e->modifiers() == Qt::NoModifier)
    {
        /*QString cmdStr = this->textCursor().block().text().mid(mPromptStringLen);
        qDebug () << cmdStr;
        mHistory.append(cmdStr);
        mHistoryItr = mHistory.end();
        QStringList args = cmdStr.split(QRegExp("\\s+"), QString::SkipEmptyParts);
        qDebug () << "args: " << args;
        QString command = *args.begin();
        qDebug () << "command is " << command;
        if(command == "compile")
        {
            args.erase(args.begin());
            CCompiler* compiler = new CCompiler(args);
            connect(compiler, SIGNAL(finished()), compiler, SLOT(deleteLater()));
            connect(compiler, SIGNAL(error(QString)), this, SLOT(appendError(QString)));
            connect(compiler, SIGNAL(log(QString)), this, SLOT(appendOutput(QString)));
            connect(compiler, SIGNAL(logHtml(QString)), this, SLOT(appendHtml(QString)));
            connect(compiler, SIGNAL(started()), this, SLOT(lock()));
            connect(compiler, SIGNAL(finished()), this, SLOT(unlock()));
            compiler->setWorkingDir(mFileSystem.getCurrentPath());
            compiler->start();
            //mController->setSolutionPath(mFileSystem.getCurrentPath() + "/app");
        }
        else if(false)//cmddata.title == "ls")
        {
//            QStringList ls = mFileSystem.getDirList();
//            for(QString str : ls)
//                appendOutput(str);
        }
        else if(command == "cd")
        {
            mFileSystem.changeDir(*(args.begin()+1));
            newCmdPrompt();
        }
        else
        {
            CSystemCmd* cmd = new CSystemCmd(QStringList() << cmdStr);
            connect(cmd, SIGNAL(finished()), cmd, SLOT(deleteLater()));
            connect(cmd, SIGNAL(error(QString)), this, SLOT(appendError(QString)));
            connect(cmd, SIGNAL(log(QString)), this, SLOT(appendOutput(QString)));
            connect(cmd, SIGNAL(finished()), this, SLOT(unlock()));
            connect(cmd, SIGNAL(started()), this, SLOT(lock()));
            cmd->setWorkingDir(mFileSystem.getCurrentPath());
            cmd->start();
        }
        //else if(!cmddata.title.isEmpty())
        //    appendError("Error. " + cmddata.title + ": unknown command");*/
    }
}

void CTerminal::tabKeyHandler()
{
    QString cmdStr = this->textCursor().block().text().mid(mPromptStringLen);
    std::cout << "cmdStr: " << cmdStr.toStdString() << std::endl;
    int idx = cmdStr.length()-1;
    while(idx >= 0 && (cmdStr[idx] != ' ' && cmdStr[idx] != 160))
    {
        --idx;
    }
    cmdStr = cmdStr.mid(idx+1);
    QPair<QStringList, int> hints = mFileSystem.getHint(cmdStr);
    qDebug () << "hints: " << hints.first;
    
    if(mTabPressCount > 1 && hints.first.size() > 1)
    {
        QTextBlock b = this->document()->lastBlock();
        for(const QString& str : hints.first)
            this->appendOutput(str);
        this->appendMain(b.text());
    }
    else if(!hints.first.isEmpty())
    {
        int lcp = 0;
        for(;(*hints.first.begin()).length() > lcp ; ++lcp)
        {
            QChar need = (*hints.first.begin())[lcp];
            bool f = true;
            for(const QString s : hints.first)
                f &= s.length() > lcp && s[lcp] == need;
            if(!f)
                break;
        }
        qDebug () << "lcp = " << lcp;
        if(lcp > 0)
        {
            QString toAppend = (*hints.first.begin()).mid(hints.second, lcp-hints.second);
            this->textCursor().insertText(toAppend);
            if(hints.first.size() == 1 && mFileSystem.isDirectory(cmdStr + toAppend))
            {
                this->textCursor().insertText("/");
            }
        }
    }
    mTabPressCount = 0;
}

void CTerminal::mousePressEvent(QMouseEvent *e)
{
    setFocus();
}

void CTerminal::mouseDoubleClickEvent(QMouseEvent *e)
{

}

void CTerminal::contextMenuEvent(QContextMenuEvent *e)
{

}

void CTerminal::newCmdPrompt()
{
    this->textCursor().block().text().clear();
    QString prompt = mPromptString + ":" + mFileSystem.getCurrentPath() + "# ";
    mPromptStringLen = prompt.length();
    appendMain(prompt);
}

QString CTerminal::preprocessMsg(const QString &msg)
{
    QString ret = msg.toHtmlEscaped();
    ret.replace(QString("\n"), QString("<br>"));
    ret.replace(QString(" "), QString("&nbsp;"));
    return ret;
}



void CTerminal::appendMain(const QString &str)
{
    this->appendHtml("<font color=" + Colors::Main.name() + ">" + preprocessMsg(str) + "</font>");
}

void CTerminal::appendOutput(const QString &str)
{
    qDebug () << "I am here";
    this->appendHtml("<font color=" + Colors::Output.name() + ">" + preprocessMsg(str) + "</font>");
}

void CTerminal::appendError(const QString &str)
{
    this->appendHtml("<font color=" + Colors::Error.name() + ">" + preprocessMsg(str) + "</font>");
}

void CTerminal::lock()
{
    mLocked = true;
}

void CTerminal::unlock()
{
    mLocked = false;
    newCmdPrompt();
}

} // namespace NController
