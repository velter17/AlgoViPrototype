#ifndef CGRAVIZSYSTEM_H
#define CGRAVIZSYSTEM_H

#include <QObject>
#include "Controller/CSystemController.h"
#include "graviz/Types.h"

class CSystemController;

class CGravizSystem : public QObject
{
    Q_OBJECT
public:
    CGravizSystem(std::shared_ptr<CSystemController> controll);

public slots:
    void setMode(TSystemMode mode);

private:
    std::shared_ptr<CSystemController> mController;
    std::shared_ptr<CGraphicView> mView;
    TSystemMode mMode;
};

#endif // CGRAVIZSYSTEM_H
