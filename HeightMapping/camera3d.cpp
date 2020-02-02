#include "camera3d.h"

const QVector3D Camera3D::LocalForward(0.0f, 0.0f, -1.0f);
const QVector3D Camera3D::LocalRight(1.0f, 0.0f, 0.0f);
const QVector3D Camera3D::LocalUp(0.0f, 1.0f, 0.0f);

const QMatrix4x4& Camera3D::toMatrix()
{
    if( m_dirty )
    {
        m_dirty = false;

        m_matrix.setToIdentity();
        m_matrix.rotate(m_rotation.conjugated());
        m_matrix.translate(-m_position);
    }

    return m_matrix;
}


void Camera3D::rotate(const QQuaternion& quat)
{
    m_dirty = true;
    m_rotation = quat * m_rotation;
}

void Camera3D::translate(const QVector3D& translation)
{
    m_dirty = true;
    m_position += translation;
}

QVector3D Camera3D::forward()
{
    return m_rotation.rotatedVector(LocalForward);
}

QVector3D Camera3D::right()
{
    return m_rotation.rotatedVector(LocalRight);
}

QVector3D Camera3D::up()
{
    return m_rotation.rotatedVector(LocalUp);
}
