#pragma once

#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include "transform3D.h"
#include "camera3d.h"


class QOpenGLShaderProgram;

class MainWindow :
        public QOpenGLWindow,
        protected QOpenGLFunctions
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

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    // OpenGLState
    QOpenGLBuffer m_vertex;                // Vertex & color buffer. Type is QOpenGLBuffer::VertexBuffer by default
    QOpenGLVertexArrayObject m_object;
    QOpenGLShaderProgram *m_program;

    // Informations shaders
    int u_modelToWorld;
    int u_worldToCamera;
    int u_cameraToView;

    QMatrix4x4 m_projection;
    Transform3D m_transform;
    Camera3D m_camera;

    // Outils privés
    void printContextInformations(); //!< print information about OpenGL context
};
