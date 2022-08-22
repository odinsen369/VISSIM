#include "cube.h"

Cube::Cube()
{

}

Cube::Cube(float x, float y, float z)
{
    createcube();
    mMatrix.translate(x,y,z);
}

Cube::~Cube() {}

void Cube::init(GLint matrixUniform)
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

void Cube::draw()
{
   glBindVertexArray( mVAO );
   glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
   glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

std::vector<Vertex> Cube::createcube()
{
    std::vector<Vertex> mVertices;
    mVertices.reserve(36);
    MakeSquare(mVertices, {0,0,-1});
    MakeSquare(mVertices, {0,0,1});
    MakeSquare(mVertices, {0,1,0});
    MakeSquare(mVertices, {0,-1,0});
    MakeSquare(mVertices, {-1,0,0});
    MakeSquare(mVertices, {1,0,0});
    return mVertices;
}

void Cube::MakeSquare(std::vector<Vertex>& mVertices, QVector3D direction)
{
    float r,g,b;
    r = abs(direction.x());
    g = abs(direction.y());
    b = abs(direction.z());

    auto center = direction*(1/2);

    auto temp = center;

    temp.setX(abs(abs(direction.x())-1));
    temp.setY(abs(abs(direction.y())-1));
    temp.setZ(abs(abs(direction.z())-1));

    auto corner1 = temp;
    corner1.setX(corner1.x() * -1);

    auto corner2 = temp;
    corner2.setY(corner2.y() * -1);

    auto corner3 = temp;
    corner3.setZ(corner3.z() * -1);

    temp.setX(abs(abs(direction.x())-1)*-1);
    temp.setY(abs(abs(direction.y())-1)*-1);
    temp.setZ(abs(abs(direction.z())-1)*-1);

    auto corner4 = temp;
    corner4.setX(corner4.x() * -1);

    auto corner5 = temp;
    corner5.setY(corner5.y() * -1);

    auto corner6 = temp;
    corner6.setZ(corner6.z() * -1);

    corner1 = corner1 + direction;
    corner2 = corner2 + direction;
    corner3 = corner3 + direction;
    corner4 = corner4 + direction;
    corner5 = corner5 + direction;
    corner6 = corner6 + direction;

    mVertices.push_back(Vertex{corner1/5, r, g, b});
    mVertices.push_back(Vertex{corner2/5, r, g, b});
    mVertices.push_back(Vertex{corner3/5, r, g, b});
    mVertices.push_back(Vertex{corner4/5, r, g, b});
    mVertices.push_back(Vertex{corner5/5, r, g, b});
    mVertices.push_back(Vertex{corner6/5, r, g, b});
}
