#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include "transform3D.h"


class QOpenGLShaderProgram;

class MainWindow : public QOpenGLWindow, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void teardownGL(); //!< Separate function to perform cleanup

protected slots:
    void update();

private:
    // OpenGLState
    QOpenGLBuffer m_vertex;
    QOpenGLVertexArrayObject m_object;
    QOpenGLShaderProgram *m_program;

    // Informations shaders
    int u_modelToWorld;
    int u_worldToView;
    QMatrix4x4 m_projection;
    Transform3D m_transform;

    // Outils privés
    void printContextInformations(); //!< print information about OpenGL context
};
#endif // MAINWINDOW_H
