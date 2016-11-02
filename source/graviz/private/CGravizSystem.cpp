#include "graviz/CGravizSystem.h"
#include <QProcess>
#include <QDebug>

CGravizSystem::CGravizSystem(std::shared_ptr<CSystemController> controll)
   : mView(new CGraphicView())
   , mController(controll)
   , mMode(TSystemMode::Interaction)
{
    mController->setModel(std::shared_ptr<CGravizSystem>(this));
    mController->setView(mView);
}


void CGravizSystem::setMode(TSystemMode mode)
{
    mMode = mode;
}
