#include "minstekvadrater.h"

MinsteKvadrater::MinsteKvadrater()
{
    construct(/*funksjon*/);
}

MinsteKvadrater::~MinsteKvadrater() { }

void MinsteKvadrater::construct(/*double f(double, double)*/)
{
    float xmin=-10.0f, xmax=10.0f, h=0.1f;
    for (auto x=xmin; x<xmax; x+=h)
    {
        float y = 0.61309*pow(x,2) -0.43928*x+0.91904;
        mVertices.push_back(Vertex{x,y,0,x,y,0});
        punkter.push_back(Vertex{x,y,0});
    }
}

//double MinsteKvadrater::funksjon(double x, double y)
//{
//    double f = 0.61309*pow(x,2) -0.43928*x+0.91904;
//    return f;
//}

void MinsteKvadrater::init(GLint matrixUniform)
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

void MinsteKvadrater::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_LINE_STRIP, 0, mVertices.size());
}



