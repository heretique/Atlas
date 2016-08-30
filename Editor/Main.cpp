#include <QGuiApplication>
#include <QDebug>
#include "MainWindow.h"

using namespace atlasEditor;

int main(int argc, char **argv)
{
    if (!qputenv("QT_QPA_UPDATE_IDLE_TIME", "0"))
        qDebug() << "Variable not set";

    QGuiApplication app(argc, argv);

    MainWindow window;
    window.resize(1024, 768);
    window.show();

    return app.exec();
}

