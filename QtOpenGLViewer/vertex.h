#ifndef VERTEX_H
#define VERTEX_H

#include <QVector3D>

// Q_DECL_CONSTEXPR : est remplacé par 'const_expr' si le compilateur le permet.
//                    Permet d'optimiser durant la compilation lorsque les valeurs sont hardcodées, ce qui sera
//                    le cas tant que les données seront spécifiées dans le code, mais qui ne servira plus quand
//                    les données seront chargés dynamiquement

class Vertex
{
public:
    // Constructeurs
    Q_DECL_CONSTEXPR Vertex();
    Q_DECL_CONSTEXPR explicit Vertex(const QVector3D &position);
    Q_DECL_CONSTEXPR Vertex(const QVector3D &position,const QVector3D &color);

    // Accesseurs / Mutateurs
    Q_DECL_CONSTEXPR const QVector3D& position() const;
    Q_DECL_CONSTEXPR const QVector3D& color() const;
    void setPosition(const QVector3D& position);
    void setColor(const QVector3D& color);

    // Pour OpenGL...
    static const int PositionTupleSize = 3;  // ...TupleSize : Nombre d'éléments présents dans la collection d'information
    static const int ColorTupleSize = 3;
    static Q_DECL_CONSTEXPR int positionOffset(); // ...OffsetSize() : décalage nécessaire dans la structure pour accéder à cette donnée
    static Q_DECL_CONSTEXPR int colorOffset();
    static Q_DECL_CONSTEXPR int stride();    // Décalage total pour accéder à l'information suivante dans un tableau de données contigues. Plus propre qu'un simple sizeof(Type)

private:
    // Utilisation de QVector3D pour ne pas avoir à définir en plus notre propre type
    QVector3D m_position;
    QVector3D m_color;
};

//------------
// INLINE
//------------

// Note: Q_MOVABLE_TYPE means it can be moved by memcpy()
Q_DECLARE_TYPEINFO(Vertex, Q_MOVABLE_TYPE);

// Constructors
Q_DECL_CONSTEXPR inline Vertex::Vertex()
{}

Q_DECL_CONSTEXPR inline Vertex::Vertex(const QVector3D &position) :
    m_position(position)
{}

Q_DECL_CONSTEXPR inline Vertex::Vertex(const QVector3D &position, const QVector3D &color) :
    m_position(position),
    m_color(color)
{}

// Accessors / Mutators
Q_DECL_CONSTEXPR inline const QVector3D& Vertex::position() const
{ return m_position; }

Q_DECL_CONSTEXPR inline const QVector3D& Vertex::color() const
{ return m_color; }

void inline Vertex::setPosition(const QVector3D& position)
{ m_position = position; }

void inline Vertex::setColor(const QVector3D& color)
{ m_color = color; }


// OpenGL Helpers
Q_DECL_CONSTEXPR inline int Vertex::positionOffset()
{ return offsetof(Vertex, m_position); }

Q_DECL_CONSTEXPR inline int Vertex::colorOffset()
{ return offsetof(Vertex, m_color); }

Q_DECL_CONSTEXPR inline int Vertex::stride()
{ return sizeof(Vertex); }

#endif // VERTEX_H
