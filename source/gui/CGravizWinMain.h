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


namespace NController {
class CTerminal;
class CSystemController;
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

    void setAlgoLabel(const QString& title, const QString& value);
    QGraphicsView* getGraphicsView();
    NController::CTerminal* getTerminal();

signals:
    void modeChanged(NGraviz::TSystemMode mode);

private: //fields
    Ui::CGravizWinMain *ui;

    QMap<QString, std::shared_ptr<QLabel>> mAlgoLabels;
};
