#include "mainwindow.h"

#include <QGuiApplication>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Set OpenGL Version information; must be set before show() is called
    // This is not explicitly neded, but it's a good way to check functionalities
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3,3);

    // set window
    MainWindow w;
    w.setFormat(format);
    w.resize(QSize(800,600));
    w.show();

    return app.exec();
}
