#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

class QOpenGLShaderProgram;

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
    // OpenGLState
    QOpenGLBuffer m_vertex;
    QOpenGLVertexArrayObject m_object;
    QOpenGLShaderProgram *m_program;

    // Outils privés
    void printContextInformations(); //!< print information about OpenGL context
};
#endif // MAINWINDOW_H
