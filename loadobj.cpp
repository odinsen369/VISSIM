#include "loadobj.h"

LoadOBJ::LoadOBJ()
{

    readFile("oblig1.txt");
}

LoadOBJ::~LoadOBJ()
{

}

void LoadOBJ::readFile(std::string filnavn) {
    std::string linje;
    int count = 0;
    std::string sammenlign = "v";
    std::ifstream fil;
    fil.open("../3Dprog22/"+filnavn); //åpner oblig1.txt

    if (!fil.is_open()) {
        std::cout << "Epic fail";
    }

    std::string test;
    while (getline(fil,linje) ){
        fil >> test;
        if (test.compare(sammenlign) == 0)
        {
            count += 1;
        }
    }
    //count*=4;
    Vertex vertex1;
    Vertex vertex2;
//    Vertex vertex3;
//    Vertex vertex4;
        mVertices.reserve(count);
        for (int i=0; i<count; i++) {
            if (test.compare(sammenlign) == 0)
            {
             fil >> vertex1;
             mVertices.push_back(vertex1); //sender vertexene fra tekstfila inn i mVertices
            }
            if (test.compare(sammenlign) != 0){
            fil >> vertex2;
            mVertices.push_back(vertex2);
        }
//            fil >> vertex3;
//            fil >> vertex4;
//            mVertices.push_back(vertex2);
//            mVertices.push_back(vertex3);
//            mVertices.push_back(vertex4);
        fil.close();
    }
    mMatrix.setToIdentity();

}

void LoadOBJ::init(GLint matrixUniform)
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

void LoadOBJ::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}
