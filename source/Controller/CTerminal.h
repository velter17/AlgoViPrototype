/**
 * Project   Graviz
 *
 * @file     CTerminal.h
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#pragma once

#include <QPlainTextEdit>
#include <QPushButton>
#include <QVector>
#include <QPair>
#include <QColor>
#include <QStringList>

#include "framework/Commands/CFileSystem.h"
#include "Controller/CSystemController.h"

namespace NController
{

class CSystemController;

class CTerminal : public QPlainTextEdit
{
    Q_OBJECT
public: // methods
    CTerminal(QWidget* parent);

    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void contextMenuEvent(QContextMenuEvent *e);

private: // methods
    QString preprocessMsg(const QString& msg);
    void newCmdPrompt();

public slots: // methods
    void appendMain(const QString& str);
    void appendOutput(const QString& str);
    void appendError(const QString& str);

private slots: // methods
    void lock();
    void unlock();
    void tabKeyHandler();

signals:
    void command(const QStringList& args);

private: // fields
    QString mPromptString;

    struct Colors
    {
        static QColor Background;
        static QColor Main;
        static QColor Output;
        static QColor Error;
    };
    QPalette mPalette;
    bool mLocked;
    NCommand::CFileSystem mFileSystem;
    int mPromptStringLen;
    QStringList mHistory;
    QStringList::iterator mHistoryItr;
    int mTabPressCount;
};

} // namespace NController
