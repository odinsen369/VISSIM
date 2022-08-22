#include "beziercurve.h"

BezierCurve::BezierCurve()
{
    Vertex v1{0,1,0,0,1,0};
    Vertex v2{1,2,0,1,2,0};
    Vertex v3{1,0,0,1,0,0};
    Vertex v4{2,1,0,0,1,0};
//    Vertex v1{3,8,0,0,1,0};
//    Vertex v2{9,7,0,1,2,0};
//    Vertex v3{2,6,0,1,0,0};
//    Vertex v4{1,5,0,0,1,0};
    mVertices.push_back(v4);
    mVertices.push_back(v3);
    mVertices.push_back(v2);
    mVertices.push_back(v1);
    makebezier(v1,v2,v3,v4);
}

BezierCurve::~BezierCurve() {}

void BezierCurve::init(GLint matrixUniform)
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

void BezierCurve::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_LINE_STRIP, 0, mVertices.size());
}

void BezierCurve::makebezier(Vertex v1, Vertex v2,Vertex v3, Vertex v4)
{
    Vertex temp{0,0,0,0,0};
    for (float x = 0; x<1; x+=0.01f)
    {
        temp = CalculateBezier(v1,v2,v3,v4,x); //sender inn punkter
        mVertices.push_back(temp);
    }
}

Vertex BezierCurve::CalculateBezier(Vertex v1,Vertex v2,Vertex v3,Vertex v4, float t)
{
    Vertex temp{0,0,1,0,1};

    temp.setX(
                        ( pow((1-t), 3) * v1.getX()         )
                    +   ( pow((1-t), 2) * 3 * t * v2.getX() )   //putter punkter inn i funksjonen for kubisk bezierkurve
                    +   ( (1 - t) * 3 * t * t *  v3.getX()  )
                    +   ( t * t * t * v4.getX()             )
                    );
    temp.setY(
                        ( pow((1-t), 3) * v1.getY()         )
                    +   ( pow((1-t), 2) * 3 * t * v2.getY() )
                    +   ( (1 - t) * 3 * t * t *  v3.getY()  )
                    +   ( t * t * t * v4.getY()             )
                    );
    return temp;
}
