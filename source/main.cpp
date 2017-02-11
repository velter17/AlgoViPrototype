#include "gui/CAlgoViWinMain.h"
#include "algovi/CAlgoViSystem.h"
#include "Controller/CSystemController.h"
#include <QThread>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::shared_ptr<CAlgoViWinMain> gui(new CAlgoViWinMain());
    gui->init();

    std::shared_ptr<NController::CSystemController> controller(
                new NController::CSystemController(gui));
    std::shared_ptr<NAlgoVi::CAlgoViSystem> algoviApp(
                new NAlgoVi::CAlgoViSystem(controller));
    gui->show();

    return a.exec();
}
