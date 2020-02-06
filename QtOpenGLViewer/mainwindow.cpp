#include <QDebug>
#include <QString>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>

#include "vertex.h"
#include "input.h"

#include "mainwindow.h"


// Triangle coloré
static const Vertex sg_verticesTriangle[] =
{
    Vertex( QVector3D(  0.00f,  0.75f, 1.0f), QVector3D( 1.0f, 0.0f, 0.0f) ),
    Vertex( QVector3D(  0.75f, -0.75f, 1.0f), QVector3D( 0.0f, 1.0f, 0.0f) ),
    Vertex( QVector3D( -0.75f, -0.75f, 1.0f), QVector3D( 0.0f, 0.0f, 1.0f) )
};

// Cube coloré
// Attention à l'ordre des vertices, car il faut que les sommets d'une face soit dans le sens antihoraire

// Front
#define VERTEX_FTR Vertex( QVector3D( 0.5f,  0.5f,  0.5f), QVector3D( 1.0f, 0.0f, 0.0f ) )
#define VERTEX_FTL Vertex( QVector3D(-0.5f,  0.5f,  0.5f), QVector3D( 0.0f, 1.0f, 0.0f ) )
#define VERTEX_FBL Vertex( QVector3D(-0.5f, -0.5f,  0.5f), QVector3D( 0.0f, 0.0f, 1.0f ) )
#define VERTEX_FBR Vertex( QVector3D( 0.5f, -0.5f,  0.5f), QVector3D( 0.0f, 0.0f, 0.0f ) )

// Back
#define VERTEX_BTR Vertex( QVector3D( 0.5f,  0.5f, -0.5f), QVector3D( 1.0f, 1.0f, 0.0f ) )
#define VERTEX_BTL Vertex( QVector3D(-0.5f,  0.5f, -0.5f), QVector3D( 0.0f, 1.0f, 1.0f ) )
#define VERTEX_BBL Vertex( QVector3D(-0.5f, -0.5f, -0.5f), QVector3D( 1.0f, 0.0f, 1.0f ) )
#define VERTEX_BBR Vertex( QVector3D( 0.5f, -0.5f, -0.5f), QVector3D( 1.0f, 1.0f, 1.0f ) )

static const Vertex sg_vertices[] =
{
    // Face 1 (Front)
      VERTEX_FTR, VERTEX_FTL, VERTEX_FBL,
      VERTEX_FBL, VERTEX_FBR, VERTEX_FTR,
    // Face 2 (Back)
      VERTEX_BBR, VERTEX_BTL, VERTEX_BTR,
      VERTEX_BTL, VERTEX_BBR, VERTEX_BBL,
    // Face 3 (Top)
      VERTEX_FTR, VERTEX_BTR, VERTEX_BTL,
      VERTEX_BTL, VERTEX_FTL, VERTEX_FTR,
    // Face 4 (Bottom)
      VERTEX_FBR, VERTEX_FBL, VERTEX_BBL,
      VERTEX_BBL, VERTEX_BBR, VERTEX_FBR,
    // Face 5 (Left)
      VERTEX_FBL, VERTEX_FTL, VERTEX_BTL,
      VERTEX_FBL, VERTEX_BTL, VERTEX_BBL,
    // Face 6 (Right)
      VERTEX_FTR, VERTEX_FBR, VERTEX_BBR,
      VERTEX_BBR, VERTEX_BTR, VERTEX_FTR
};

#undef VERTEX_BBR
#undef VERTEX_BBL
#undef VERTEX_BTL
#undef VERTEX_BTR

#undef VERTEX_FBR
#undef VERTEX_FBL
#undef VERTEX_FTL
#undef VERTEX_FTR



MainWindow::MainWindow()
{
    m_transform.translate(0.0f,0.0f,-5.0f);// L'objet est reculé de 5 unités
}

MainWindow::~MainWindow()
{
    makeCurrent();
    teardownGL();
}

void MainWindow::initializeGL()
{
    // Perform initialization with the current OpenGL context
    initializeOpenGLFunctions();

    // Connexion signal et slot pour être averti d'une changement de frame. Par defaut le VSync
    // est activé dans Qt. Il est aussi possible de s'appuyer sur QTimer, mais en programmation
    // de rendu moderne, il est d'usage de s'appuyer sur la synchro d'écran
    connect(this, SIGNAL(frameSwapped()),this,SLOT(update()));

    printContextInformations();

    // Ne faire le rendu que des faces qui sont affichée en sens antihoraire
    glEnable(GL_CULL_FACE);

    // Simply set the clear color
    glClearColor(0.0f,0.0f,0.0f,1.0f);

    // Initialisation spécifique à l'application
    {
        //--- Création du shader (ne pas libérer tant que le VertexArrayObject n'est pas créé)

        m_program = new QOpenGLShaderProgram();   // Seule partie créée par 'new' car passera de main en main. 'delete' de l'instance libérera le shader de la mémoire GPU
        m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simple.vert"); // Le Vertex shader travaille avec notre type Vertex, le Fragment shader prendrea la sortie du Vertex shader, la sortie du Fragment se fait vers vers l'écran
        m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.frag");
        m_program->link();  // 'link()' rassemble les shaders. Le retour devrait être testé (sera fait plus tard)
        m_program->bind();  // 'bind()' pour que ce shader soit celui qui est actif


        //--- Définition des Uniforms des shaders

        u_modelToWorld = m_program->uniformLocation("modelToWorld");
        u_worldToCamera = m_program->uniformLocation("worldToCamera");
        u_cameraToView = m_program->uniformLocation("cameraToView");


        //--- Création du buffer (ne pas libérer tant que le VertexArrayObject n'est pas créé)

        m_vertex.create();   // Creation du bufferpour une allocation dynamique
        m_vertex.bind();     // Marquer comme buffer courant
        m_vertex.setUsagePattern(QOpenGLBuffer::StaticDraw);  // Les données ne changeront pas, donc rendu statique
        m_vertex.allocate(sg_vertices,sizeof(sg_vertices)); // Allocation et initisalisation


        //--- Création du VertexArrayObject (VAO)

        m_object.create();
        m_object.bind();
        m_program->enableAttributeArray(0);  // Attribut 0 en position, Attribut 1 en couleur (les  ...Offets, ...TupleSize et stride simplifient l'écriture)
        m_program->enableAttributeArray(1);
        m_program->setAttributeBuffer(0, GL_FLOAT,Vertex::positionOffset(),Vertex::PositionTupleSize,Vertex::stride());
        m_program->setAttributeBuffer(1, GL_FLOAT,Vertex::colorOffset(),Vertex::ColorTupleSize,Vertex::stride());
        m_object.release(); // Libération (unbind all), dans le sens inverse de la création. 'release' (et non 'unbind') est l'opposé du 'bind'
        m_vertex.release();
        m_program->release();
    }
}

void MainWindow::resizeGL(int w, int h)
{
    m_projection.setToIdentity();
    m_projection.perspective( 45.0f, w/float(h), 0.0f, 1000.0f );
}

void MainWindow::paintGL()
{
    // Clear with set clear color
    glClear(GL_COLOR_BUFFER_BIT);

    // Rendu utilisant le shader. Très simple car s'appuie sur un VAO
    m_program->bind();
    m_program->setUniformValue( u_worldToCamera, m_camera.toMatrix() );
    m_program->setUniformValue( u_cameraToView, m_projection );
    {
        m_object.bind();
        m_program->setUniformValue( u_modelToWorld, m_transform.toMatrix() );
        glDrawArrays( GL_TRIANGLES, 0, sizeof(sg_vertices)/sizeof(sg_vertices[0]) );
        m_object.release();
    }

    m_program->release();
}


void MainWindow::update()
{
    Input::update();

    // Récupération des entrées
    if(Input::buttonPressed(Qt::RightButton))
    {
        static const float transSpeed = 0.5f; // A modifier par une valeur dépendante du taux de rafraichissement
        static const float rotSpeed = 0.5f;

        m_camera.rotate( -rotSpeed * Input::mouseDelta().x(), Camera3D::LocalUp);
        m_camera.rotate( -rotSpeed * Input::mouseDelta().y(), m_camera.right());

        QVector3D translation;
        if( Input::keyPressed(Qt::Key_Z))
        {
            translation += m_camera.forward();
        }
        if( Input::keyPressed(Qt::Key_S))
        {
            translation -= m_camera.forward();
        }
        if( Input::keyPressed(Qt::Key_D))
        {
            translation += m_camera.right();
        }
        if( Input::keyPressed(Qt::Key_Q))
        {
            translation -= m_camera.right();
        }
        if( Input::keyPressed(Qt::Key_E))
        {
            translation += m_camera.up();
        }
        if( Input::keyPressed(Qt::Key_A))
        {
            translation -= m_camera.up();
        }

        m_camera.translate( transSpeed*translation);
    }

    // Mise à jour de l'objet
    m_transform.rotate( 1.0f, QVector3D( 0.4f, 0.3f, 0.3f ) );

    // Demande de rafraichissement
    QOpenGLWindow::update();
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
  if (event->isAutoRepeat())
  {
    event->ignore();
  }
  else
  {
    Input::registerKeyPress(event->key());
  }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
  if (event->isAutoRepeat())
  {
    event->ignore();
  }
  else
  {
    Input::registerKeyRelease(event->key());
  }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
  Input::registerMousePress(event->button());
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
  Input::registerMouseRelease(event->button());
}


void MainWindow::teardownGL()
{
    // Destruction des informations OpenGL
    m_object.destroy();
    m_vertex.destroy();
    delete m_program;
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
