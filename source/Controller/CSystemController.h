/**
 * Project   AlgoVi
 *
 * @file     CSystemController.h
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#pragma once

#include <QObject>

#include "gui/CAlgoViWinMain.h"
#include "Controller/CTerminal.h"
#include "View/CGraphicView.h"
#include "algovi/CAlgoViSystem.h"

class CAlgoViWinMain;
namespace NAlgoVi
{
class CAlgoViSystem;
}

namespace NController
{

class CSystemController : public QObject
{
    Q_OBJECT
public:
    CSystemController(std::shared_ptr<CAlgoViWinMain> GUI);

    /**
     * @brief setModel
     * @param model
     */
    void setModel(std::shared_ptr<NAlgoVi::CAlgoViSystem> model);

    /**
     * @brief setView
     * @param view
     */
    void setView(std::shared_ptr<NView::CGraphicView> view);

public slots:
    void handleCommand(NController::TTerminalCommandType type, const QString& cmd);
    void handleLog(QString msg);
    void handleError(QString msg);
    void handleLogHtml(QString msg);
    void unlock();
    void setAppMode();
    void setQuestionMode();
    void exit();

private:
    std::shared_ptr<CAlgoViWinMain> mGUI;
    std::shared_ptr<NView::CGraphicView> mView;
    std::shared_ptr<NAlgoVi::CAlgoViSystem> mModel;
};

} // namespace NController
