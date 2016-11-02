#ifndef CSYSTEMCONTROLLER_H
#define CSYSTEMCONTROLLER_H

#include "gui/CGravizWinMain.h"
#include "Controller/CTerminal.h"
#include "View/CGraphicView.h"
#include "graviz/CGravizSystem.h"
#include <QObject>

class CGravizWinMain;
class CGravizSystem;

class CSystemController
{
public:
    CSystemController(std::shared_ptr<CGravizWinMain> GUI);

    /**
     * @brief setModel
     * @param model
     */
    void setModel(std::shared_ptr<CGravizSystem> model);

    /**
     * @brief setView
     * @param view
     */
    void setView(std::shared_ptr<CGraphicView> view);

private:
    std::shared_ptr<CGravizWinMain> mGUI;
    std::shared_ptr<CGraphicView> mView;
    std::shared_ptr<CGravizSystem> mModel;
};

#endif // CSYSTEMCONTROLLER_H
