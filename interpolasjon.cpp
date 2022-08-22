#include "interpolasjon.h"

Interpolasjon::Interpolasjon()
{
    construct();
}

Interpolasjon::~Interpolasjon() { }

void Interpolasjon::construct()
{
    float xmin=0.0f, xmax=10.0f, h=0.1f;
    for (auto x=xmin; x<xmax; x+=h)
    {
        float y = (7/6.f)*pow(x,3) - 9*pow(x,2) + (131/6.f)*x-12; //bruk f for at det skal bli float for å slippe int avrunning
        mVertices.push_back(Vertex{x,y,x,y,0});
    }
}

//double Interpolasjon::funksjon(double x, double y)
//{
//    double f = 7/6*pow(x,3) - 9*pow(x,2) + 131/6*x -12;
//    return f;
//}

void Interpolasjon::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;
    initializeOpenGLFunctions();

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glBufferData(GL_ARRAY_BUFFER,
                 mVertices.size()* sizeof (Vertex),
                 mVertices.data(),
                 GL_STATIC_DRAW
                 );

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(
                0,                  // attribute. No particular reason for 0, but must match layout(location = 0) in the vertex shader.
                3,                  // size / number of elements of data type
                GL_FLOAT,           // data type
                GL_FALSE,           // normalize data
                sizeof(Vertex),  // stride
                reinterpret_cast<GLvoid*>(0));          // array buffer offset
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof (Vertex),  reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

void Interpolasjon::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_LINES, 0, mVertices.size());
    glDrawArrays(GL_LINES, 1, mVertices.size()-1);
}
