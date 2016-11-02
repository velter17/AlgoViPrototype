#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>

#include "View/CGraphicView.h"
#include "Controller/CSystemController.h"
#include "Controller/CTerminal.h"
#include "graviz/Types.h"
#include <QLabel>
#include <QMap>

class CSystemController;
class CTerminal;

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
    CTerminal* getTerminal();

signals:
    void modeChanged(TSystemMode mode);

private: //fields
    Ui::CGravizWinMain *ui;

    QMap<QString, std::shared_ptr<QLabel>> mAlgoLabels;
};

#endif // MAINWINDOW_H
