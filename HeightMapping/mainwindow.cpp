#include "mainwindow.h"
#include <QDebug>
#include <QString>

MainWindow::~MainWindow()
{
    makeCurrent();
    teardownGL();
}

void MainWindow::initializeGL()
{
    // Perform initialization with the current OpenGL context
    initializeOpenGLFunctions();

    printContextInformations();

    // Simply set the clear color
    glClearColor(0.0f,0.0f,0.0f,1.0f);
}
void MainWindow::resizeGL(int w, int h)
{
    (void)w;
    (void)h;
}
void MainWindow::paintGL()
{
    // Clear with set clear color
    glClear(GL_COLOR_BUFFER_BIT);
}
void MainWindow::teardownGL()
{
    // no resources allocated for the moment
}

void MainWindow::printContextInformations()
{
    QString glType;
    QString glVersion;
    QString glProfile;

    glType = (context()->isOpenGLES())?"OpenGL ES":"OpenGL";
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
    switch(format().profile())
    {
    CASE(NoProfile);
    CASE(CoreProfile);
    CASE(CompatibilityProfile);
    }
#undef CASE

    // qPrintable is used to avoid string surrounded by quotation marks
    qDebug()<<qPrintable(glType)<<qPrintable(glVersion)<<"("<<qPrintable(glProfile)<<")";
}
