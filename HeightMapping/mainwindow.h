#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>

class MainWindow : public QOpenGLWindow, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    ~MainWindow();

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void teardownGL(); //!< Separate function to perform cleanup

private:
    void printContextInformations(); //!< print information about OpenGL context
};
#endif // MAINWINDOW_H
