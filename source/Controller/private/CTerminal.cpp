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
#include <QScrollBar>

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
    , mTabPressCount(0)
{

    mState = TTerminalState::CommandInput;
    mHistoryItr = mHistory.end();

    mPalette.setColor(QPalette::Base, Colors::Background);
    mPalette.setColor(QPalette::Text, Colors::Main);
    setPalette(mPalette);

    newCmdPrompt();

    QFont font = this->font();
    font.setFixedPitch(true);
    this->setFont(font);
    qDebug () << "Fixed pitchA === " << this->font().fixedPitch();
    qDebug () << "Fixed pitchB === " << this->fontInfo().fixedPitch();
    QFont font1("monospace");
    QFontInfo info(font1);
    this->setFont(font1);
    qDebug () << "!!!" << info.fixedPitch();
}

void CTerminal::keyPressEvent(QKeyEvent *e)
{
    this->verticalScrollBar()->setValue(verticalScrollBar()->maximum());

    if(mState == TTerminalState::InsideApp)
    {
        if(e->key() == Qt::Key_C && e->modifiers() == Qt::ControlModifier)
        {
            this->textCursor().insertText("^C");
            lock();
            command(TTerminalCommandType::System, "^C");
        }
        if(e->key() >= 0x20 && e->key() <= 0x7e
               && (e->modifiers() == Qt::NoModifier
                   || e->modifiers() == Qt::ShiftModifier
                   || e->modifiers() == Qt::KeypadModifier))
                    QPlainTextEdit::keyPressEvent(e);
        if(e->key() == Qt::Key_Backspace
               && e->modifiers() == Qt::NoModifier
               && textCursor().positionInBlock() > 0)
                QPlainTextEdit::keyPressEvent(e);
        if((e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
                && (e->modifiers() == Qt::NoModifier
                    || e->modifiers() == Qt::KeypadModifier))
        {
            QString cmdStr = this->textCursor().block().text();
            qDebug () << "cmdStr == " << cmdStr;
            this->textCursor().insertBlock();
            emit command(TTerminalCommandType::Application, cmdStr);
            //QPlainTextEdit::keyPressEvent(e);
        }
        return;
    }

    /* Single character */
    if(e->key() >= 0x20 && e->key() <= 0x7e
           && (e->modifiers() == Qt::NoModifier
               || e->modifiers() == Qt::ShiftModifier
               || e->modifiers() == Qt::KeypadModifier))
                //QPlainTextEdit::keyPressEvent(e);
    {
        this->textCursor().insertHtml(preprocessMsg(e->text()));
    }

    /* Delete one character by backspace key */
    if(e->key() == Qt::Key_Backspace
           && e->modifiers() == Qt::NoModifier
           && textCursor().positionInBlock() > mPromptStringLen)
    {
        QPlainTextEdit::keyPressEvent(e);
    }

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
        qDebug () << "TAB!";
        if(mTabPressCount == 0)
        {
            QTimer::singleShot(150, this, SLOT(tabKeyHandler()));
        }
        ++mTabPressCount;
    }

    /* Execute command */
    if((e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
            && (e->modifiers() == Qt::NoModifier
                || e->modifiers() == Qt::KeypadModifier))
    {
        QString cmdStr = this->textCursor().block().text().mid(mPromptStringLen);
        qDebug () << "execute command : " << cmdStr;
        mHistory.append(cmdStr);
        mHistoryItr = mHistory.end();
        this->textCursor().insertBlock();
        emit command(TTerminalCommandType::System, cmdStr);
        //QPlainTextEdit::keyPressEvent(e);
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
    QPair<QStringList, int> hints = NCommand::CFileSystem::getInstance().getHint(cmdStr);
    qDebug () << "hints: " << hints.first << ", tabs=" << mTabPressCount;

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
            if(hints.first.size() == 1 &&
                    NCommand::CFileSystem::getInstance().isDirectory(cmdStr + toAppend))
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
    //this->textCursor().block().text().clear();
    QString prompt = mPromptString + ":" + NCommand::CFileSystem::getInstance().getCurrentPath() + "# ";
    mPromptStringLen = prompt.length();
    appendMain(prompt);
}

void CTerminal::appendString(const QString &str)
{
    this->appendHtml(str);
    //this->textCursor().insertBlock();
    //this->appendHtml("<font color=" + Colors::Main.name() + ">^</font>");
    //this->appendMain("");
    this->verticalScrollBar()->setValue(verticalScrollBar()->maximum());
}

void CTerminal::insertString(const QString &str)
{
    //QTextCursor p = QTextCursor(document()->lastBlock());
    //p.insertHtml(str);
    //p.insertText(str);
    //this->textCursor().insertBlock();
    QTextBlock block = document()->lastBlock();
    QTextCursor p(block);
    p.select(QTextCursor::BlockUnderCursor);
    p.removeSelectedText();
    this->textCursor().deletePreviousChar();
    this->appendHtml(str);
    this->textCursor().insertBlock();
    this->verticalScrollBar()->setValue(verticalScrollBar()->maximum());
}

QString CTerminal::preprocessMsg(const QString &msg)
{
    QString ret = msg.toHtmlEscaped();
    if(ret.length() > 0 && *(ret.end()-1) == '\n')
        ret.chop(1);
    ret.replace(QString("\n"), QString("<br>"));
    ret.replace(QString(" "), QString("&nbsp;"));
    return ret;
}



void CTerminal::appendMain(const QString &str)
{
    //appendString("<font color=" + Colors::Main.name() + ">" + preprocessMsg(str) + "</font>");
    appendString(str);
}

void CTerminal::appendOutput(const QString &str)
{
    insertString("<font color=" + Colors::Output.name() + ">" + preprocessMsg(str) + "</font>");
}

void CTerminal::appendError(const QString &str)
{
    insertString("<font color=" + Colors::Error.name() + ">" + preprocessMsg(str) + "</font>");
}

void CTerminal::question(const QString &str, const std::vector<QString> &answers)
{

}

void CTerminal::lock()
{
    qDebug () << "CTerminal> lock";
    mStateBeforeLock = mState;
    mState = TTerminalState::Locked;
}

void CTerminal::unlock()
{
    qDebug () << "CTerminal> unlock";
    mState = mStateBeforeLock;
    newCmdPrompt();
}

void CTerminal::appMode()
{
    qDebug () << "CTerminal> appMode";
    mStateBeforeLock = mState;
    mState = TTerminalState::InsideApp;
}

} // namespace NController
