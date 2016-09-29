#include <QApplication>

#include "appcontroller.h"

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    AppController *controller = new AppController();
    controller->startApplication();

    return app.exec();
}
