#include "gui/CGravizWinMain.h"
#include "graviz/CGravizSystem.h"
#include "Controller/CSystemController.h"
#include <QApplication>

int main(int argc, char *argv[])
{   
    QApplication a(argc, argv);
    std::shared_ptr<CGravizWinMain> gui(new CGravizWinMain());
    gui->init();

    std::shared_ptr<CSystemController> controller(new CSystemController(gui));
    std::shared_ptr<CGravizSystem> gravizApp(new CGravizSystem(controller));

    gui->show();

    return a.exec();
}
