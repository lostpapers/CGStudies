#pragma once

#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>

class Camera3D
{
public:

    static const QVector3D LocalForward;
    static const QVector3D LocalUp;
    static const QVector3D LocalRight;

    Camera3D();

    void rotate( float angle, const QVector3D& axis);
    void rotate( const QQuaternion& quat);
    void translate( const QVector3D& translation );

    QVector3D forward();
    QVector3D right();
    QVector3D up();

    const QMatrix4x4& toMatrix();

private:
    bool m_dirty;

    QQuaternion m_rotation;
    QVector3D m_position;
    QMatrix4x4 m_matrix;
};

inline Camera3D::Camera3D() :
    m_dirty( true )
{}

inline void Camera3D::rotate(float angle, const QVector3D& axis)
{
    rotate(QQuaternion::fromAxisAndAngle( axis, angle));
}
