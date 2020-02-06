#include "transform3D.h"

void Transform3D::translate(const QVector3D& vec3)
{
    m_dirty = true;
    m_translation += vec3;
}

void Transform3D::rotate(const QQuaternion &quat)
{
    m_dirty = true;
    m_rotation = quat * m_rotation;
}

const QMatrix4x4& Transform3D::toMatrix()
{
    if( m_dirty)
    {
        m_dirty = false;
        m_matrix.setToIdentity();
        m_matrix.translate(m_translation);
        m_matrix.rotate(m_rotation);
        m_matrix.scale(m_scale);
    }

    return m_matrix;
}
