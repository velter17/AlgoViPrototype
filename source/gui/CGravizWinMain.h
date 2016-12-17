/**
 * Project   Graviz
 *
 * @file     CGravizWinMain.h
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#pragma once

#include <QMainWindow>
#include <memory>
#include <QLabel>
#include <QMap>

#include "View/CGraphicView.h"
#include "Controller/CSystemController.h"
#include "Controller/CTerminal.h"
#include "graviz/Types.h"
#include "Controller/Types.h"


namespace NController {
class CTerminal;
enum class TTerminalCommandType;
}

namespace Ui {
class CGravizWinMain;
}

class CGravizWinMain : public QMainWindow
{
    Q_OBJECT

public: // methids
    explicit CGravizWinMain(QWidget *parent = 0);
    void init();
    ~CGravizWinMain();

    QGraphicsView* getGraphicsView();
    void lock();
    void unlock();
    void appMode();
    void questionMode();
    void handleLog(QString msg);
    void handleError(QString msg);
    void handleLogHtml(QString msg);

signals:
    void modeChanged(NGraviz::TSystemMode mode);
    void newCommand(NController::TTerminalCommandType type, const QString& cmd);

private: //fields
    Ui::CGravizWinMain *ui;

    QMap<QString, std::shared_ptr<QLabel>> mAlgoLabels;
};
