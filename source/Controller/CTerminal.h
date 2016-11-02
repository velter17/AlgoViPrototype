#ifndef CTERMINAL_H
#define CTERMINAL_H

#include <QPlainTextEdit>
#include <QPushButton>
#include <QVector>
#include <QPair>
#include <QColor>
#include <QStringList>
#include "framework/Commands/CFileSystem.h"
#include "Controller/CSystemController.h"

class CSystemController;

class CTerminal : public QPlainTextEdit
{
    Q_OBJECT
public:

    CTerminal(QWidget* parent);

    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void contextMenuEvent(QContextMenuEvent *e);

    void setController(CSystemController* ptr);

private:
    QString preprocessMsg(const QString& msg);
    void newCmdPrompt();
public slots:
    void appendMain(const QString& str);
    void appendOutput(const QString& str);
    void appendError(const QString& str);
private slots:
    void lock();
    void unlock();
    void tabKeyHandler();

private:
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

    CFileSystem mFileSystem;
    int mPromptStringLen;

    QStringList mHistory;
    QStringList::iterator mHistoryItr;

    CSystemController* mController;

    int mTabPressCount;
};

#endif // CTERMINAL_H
