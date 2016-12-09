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
#include "Controller/ITerminal.h"

namespace NController
{

class CTerminal : public QPlainTextEdit, ITerminal
{
    Q_OBJECT
public: // methods
    CTerminal(QWidget* parent);

    void keyPressEvent(QKeyEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;
    void contextMenuEvent(QContextMenuEvent *e) override;

private: // methods
    QString preprocessMsg(const QString& msg);
    void newCmdPrompt();
    void appendString(const QString& str);
    void insertString(const QString& str);
    void appendMain(const QString& str);

public slots: // interface methods
    void appendOutput(const QString& str) override;
    void appendError(const QString& str) override;
    void question(const QString& str, const std::vector <QString> &answers) override;
    void lock() override;
    void unlock() override;
    void appMode() override;
    void questionMode() override;

private slots: // methods
    void tabKeyHandler();

signals:
    void command(TTerminalCommandType type, const QString& cmd);

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
    int mPromptStringLen;
    QStringList mHistory;
    QStringList::iterator mHistoryItr;
    int mTabPressCount;
    QString lastCmd;
    TTerminalState mStateBeforeLock;

    int mLastWriter;
    QString mInputBuffer;
    bool mNewLineFlag;
};

} // namespace NController
