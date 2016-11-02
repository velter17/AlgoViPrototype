#include "Controller/CSystemController.h"
#include <QGraphicsView>

CSystemController::CSystemController(std::shared_ptr<CGravizWinMain> GUI)
    : mGUI(GUI)
    , mView(nullptr)
    , mModel(nullptr)
{
    mGUI->setAlgoLabel("Algorithm",     "not specified");
    mGUI->setAlgoLabel("Type",          "not specified");
    mGUI->setAlgoLabel("InputType",     "not specified");
    mGUI->setAlgoLabel("OutputType",    "not specified");
}

void CSystemController::setModel(std::shared_ptr<CGravizSystem> model)
{
    mModel = model;
}

void CSystemController::setView(std::shared_ptr<CGraphicView> view)
{
    mView = view;
    mGUI->getGraphicsView()->setScene(mView.get());
}
