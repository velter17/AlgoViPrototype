/**
 * Project   Graviz
 *
 * @file     CSystemController.h
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#pragma once

#include <QObject>

#include "gui/CGravizWinMain.h"
#include "Controller/CTerminal.h"
#include "View/CGraphicView.h"
#include "graviz/CGravizSystem.h"

class CGravizWinMain;
namespace NGraviz
{
class CGravizSystem;
}

namespace NController
{

class CSystemController : public QObject
{
    Q_OBJECT
public:
    CSystemController(std::shared_ptr<CGravizWinMain> GUI);

    /**
     * @brief setModel
     * @param model
     */
    void setModel(std::shared_ptr<NGraviz::CGravizSystem> model);

    /**
     * @brief setView
     * @param view
     */
    void setView(std::shared_ptr<NView::CGraphicView> view);

public slots:
    void handleCommand(QString cmd);
    void handleLog(QString msg);
    void handleError(QString msg);
    void handleLogHtml(QString msg);
    void unlock();

private:
    std::shared_ptr<CGravizWinMain> mGUI;
    std::shared_ptr<NView::CGraphicView> mView;
    std::shared_ptr<NGraviz::CGravizSystem> mModel;
};

} // namespace NController
