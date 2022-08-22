#include "light.h"
#include "renderwindow.h"
#include "meshgenerator.h"

Light::Light()
{
//    mVertices.insert( mVertices.end(),
//    {
//     Vertex{QVector3D(-0.25f, -0.25f,  0.25f),    QVector3D(0.8f, 0.8f, 0.3f),  QVector2D(0.f,  0.f)},   //Left low
//     Vertex{QVector3D( 0.25f, -0.25f,  0.25f),    QVector3D(0.8f, 0.8f, 0.3f),  QVector2D(1.f,  0.f)},   //Right low
//     Vertex{QVector3D( 0.0f,  0.25f,  0.0f),      QVector3D(0.8f, 0.8f, 0.3f),  QVector2D(0.5f, 0.5f)},  //Top
//     Vertex{QVector3D( 0.0f, -0.25f, -0.25f),     QVector3D(0.8f, 0.8f, 0.3f),  QVector2D(0.5f, 0.5f)}   //Back low
//     });

//    mIndices.insert( mIndices.end(),
//    { 0, 1, 2,
//      1, 3, 2,
//      3, 0, 2,
//      0, 3, 1
//    });

    setVertices(MeshGenerator::GenerateOktahedron(2));
    mPosition.translate(mOrbitPoint);
    mMatrix.setToIdentity();
}

void Light::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)( 6 * sizeof( GLfloat ) ));
    glEnableVertexAttribArray(2);

    //Second buffer - holds the indices (Element Array Buffer - EAB):
    glGenBuffers(1, &mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Light::draw()
{
    glBindVertexArray( mVAO );
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
    glBindVertexArray(0);
}

void Light::orbit(float dt)
{
    mPosition.setToIdentity();
    mPosition.translate(mOrbitPoint);
    mRotation.rotate(dt * 45, {0, 0, 1});
    mMatrix = mPosition * mRotation;
    mMatrix.translate({20, 0, 0});
}
