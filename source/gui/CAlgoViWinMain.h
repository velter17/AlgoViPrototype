/**
 * Project   AlgoVi
 *
 * @file     CAlgoViWinMain.h
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
#include "algovi/Types.h"
#include "Controller/Types.h"


namespace NController {
class CTerminal;
enum class TTerminalCommandType;
}

namespace Ui {
class CAlgoViWinMain;
}

class CAlgoViWinMain : public QMainWindow
{
    Q_OBJECT

public: // methids
    explicit CAlgoViWinMain(QWidget *parent = 0);
    void init();
    ~CAlgoViWinMain();

    QGraphicsView* getGraphicsView();
    void lock();
    void unlock();
    void appMode();
    void questionMode();
    void handleLog(QString msg);
    void handleError(QString msg);
    void handleLogHtml(QString msg);

signals:
    void modeChanged(NAlgoVi::TSystemMode mode);
    void newCommand(NController::TTerminalCommandType type, const QString& cmd);

private: //fields
    Ui::CAlgoViWinMain *ui;

    QMap<QString, std::shared_ptr<QLabel>> mAlgoLabels;
};
