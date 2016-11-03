/**
 * Project   Graviz
 *
 * @file     CGravizSystem.h
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#pragma once

#include <QObject>

#include "Controller/CSystemController.h"
#include "graviz/Types.h"

namespace NGraviz
{

class CGravizSystem : public QObject
{
    Q_OBJECT
public:
    CGravizSystem(std::shared_ptr<NController::CSystemController> controll);

public slots:
    void setMode(TSystemMode mode);

private:
    std::shared_ptr<NController::CSystemController> mController;
    std::shared_ptr<NView::CGraphicView> mView;
    TSystemMode mMode;
};

} // namespace NGraviz
