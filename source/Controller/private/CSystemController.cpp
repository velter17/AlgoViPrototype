/**
 * Project   Graviz
 *
 * @file     CSystemController.cpp
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#include <QGraphicsView>

#include "Controller/CSystemController.h"

namespace NController
{

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

void CSystemController::setModel(std::shared_ptr<NGraviz::CGravizSystem> model)
{
    mModel = model;
}

void CSystemController::setView(std::shared_ptr<NView::CGraphicView> view)
{
    mView = view;
    mGUI->getGraphicsView()->setScene(mView.get());
}

} // namespace NController
