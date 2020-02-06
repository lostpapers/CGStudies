#pragma once

#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>

class Transform3D
{
public:
    // Constructeur
    Transform3D();

    void translate( float x, float y, float z );
    void translate( const QVector3D& vec3 );

    void rotate( float angle, const QVector3D& vec3 );
    void rotate( const QQuaternion& quat );

    const QMatrix4x4& toMatrix();

private:
  bool m_dirty; // Optimisation pour ne pas à avoir à recréer la matrice résultante à chaque fois
  QVector3D m_translation;
  QVector3D m_scale;
  QQuaternion m_rotation;

  QMatrix4x4 m_matrix;
};

inline Transform3D::Transform3D() :
    m_dirty(true),
    m_scale(1.0f,1.0f,1.0f)
{
}

inline void Transform3D::translate(float dx, float dy,float dz)
{
    translate(QVector3D(dx, dy, dz));
}

inline void Transform3D::rotate(float aScalar, const QVector3D& aVector )
{
    rotate(QQuaternion::fromAxisAndAngle(aVector, aScalar));
}
