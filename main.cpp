#include <QApplication>

#include "appcontroller.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    AppController *controller = new AppController();
    controller->startApplication();

    return app.exec();
}
