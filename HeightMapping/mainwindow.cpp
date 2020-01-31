#include "mainwindow.h"
#include <QDebug>
#include <QString>
#include <QOpenGLShaderProgram>

#include "vertex.h"

// Triangle coloré
static const Vertex sg_vertices[] =
{
    Vertex( QVector3D(  0.00f,  0.75f, 1.0f), QVector3D( 1.0f, 0.0f, 0.0f) ),
    Vertex( QVector3D(  0.75f, -0.75f, 1.0f), QVector3D( 0.0f, 1.0f, 0.0f) ),
    Vertex( QVector3D( -0.75f, -0.75f, 1.0f), QVector3D( 0.0f, 0.0f, 1.0f) )
};






MainWindow::~MainWindow()
{
    makeCurrent();
    teardownGL();
}

void MainWindow::initializeGL()
{
    // Perform initialization with the current OpenGL context
    initializeOpenGLFunctions();

    printContextInformations();

    // Simply set the clear color
    glClearColor(0.0f,0.0f,0.0f,1.0f);

    // Initialisation spécifique à l'application
    {
        //--- Création du shader (ne pas libérer tant que le VertexArrayObject n'est pas créé)

        // Seule partie créée par 'new' car passera de main en main. 'delete' de l'instance libérera le shader de la mémoire GPU
        m_program = new QOpenGLShaderProgram();

        // Le Vertex shader travaille avec notre type Vertex, le Fragment shader prendrea la sortie du Vertex shader, la sortie du Fragment se fait vers vers l'écran
        m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simple.vert");
        m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.frag");

        // 'link()' rassemble les shaders. Le retour devrait être testé (sera fait plus tard)
        m_program->link();

        // 'bind()' pour que ce shader soit celui qui est actif
        m_program->bind();


        //--- Création du buffer (ne pas libérer tant que le VertexArrayObject n'est pas créé)

        // Creation du bufferpour une allocation dynamique
        m_vertex.create();

        // Buffer courant
        m_vertex.bind();

        // Les données ne changeront pas, donc rendu statique
        m_vertex.setUsagePattern(QOpenGLBuffer::StaticDraw);

        // Allocation et initisalisation
        m_vertex.allocate(sg_vertices,sizeof(sg_vertices));


        //--- Création du VertexArrayObject (VAO)

        // Creation
        m_object.create();

        // Objet courant
        m_object.bind();

        // Attribut 0 en position, Attribut 1 en couleur (les  ...Offets, ...TupleSize et stride simplifient l'écriture)
        m_program->enableAttributeArray(0);
        m_program->enableAttributeArray(1);
        m_program->setAttributeBuffer(0, GL_FLOAT,Vertex::positionOffset(),Vertex::PositionTupleSize,Vertex::stride());
        m_program->setAttributeBuffer(1, GL_FLOAT,Vertex::colorOffset(),Vertex::ColorTupleSize,Vertex::stride());

        // Libération (unbind all), dans le sens inverse de la création. 'release' (et non 'unbind') est l'opposé du 'bind'
        m_object.release();
        m_vertex.release();
        m_program->release();
    }
}
void MainWindow::resizeGL(int w, int h)
{
    (void)w;
    (void)h;
}
void MainWindow::paintGL()
{
    // Clear with set clear color
    glClear(GL_COLOR_BUFFER_BIT);

    // Rendu utilisant le shader. Très simple car s'appuie sur un VAO
    m_program->bind();
    {
        m_object.bind();
        glDrawArrays(GL_TRIANGLES,0,sizeof(sg_vertices)/sizeof(sg_vertices[0]));
        m_object.release();
    }

    m_program->release();
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
