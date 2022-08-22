#include "objmesh.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

ObjMesh::ObjMesh(std::string filnavn)
{
    readFile(filnavn);
    mMatrix.setToIdentity();
}

ObjMesh::~ObjMesh()
{}


void ObjMesh::init(GLint matrixUniform)
{
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

void ObjMesh::readFile(std::string filnavn)
{
    std::ifstream fileIn;
    fileIn.open(filnavn, std::ifstream::in);
    if (!fileIn)
        qDebug() << "kan ikke åpne fil fra: " << QString::fromStdString(filnavn);

    std::string enLinje;
    std::string ettOrd;

    std::vector<QVector3D> tempXYZ;
    std::vector<QVector3D> tempRGB;
    std::vector<QVector2D> tempUVs;

    unsigned int tempIndex = 0;

    while (std::getline(fileIn, enLinje))
    {
        std::stringstream sStream;

        sStream << enLinje;

        ettOrd = "";
        sStream >> ettOrd;

        if (ettOrd == "#")
            continue;
        if (ettOrd == "")
            continue;
        if (ettOrd == "v")
        {
            QVector3D tempVertex;
            sStream >> ettOrd;
            tempVertex.setX(std::stof(ettOrd));
            sStream >> ettOrd;
            tempVertex.setY(std::stof(ettOrd));
            sStream >> ettOrd;
            tempVertex.setZ(std::stof(ettOrd));

            tempXYZ.push_back(tempVertex);
            continue;
        }
        if (ettOrd == "vt")
        {
            QVector2D tempUV;
            sStream >> ettOrd;
            tempUV.setX(std::stof(ettOrd));
            sStream >> ettOrd;
            tempUV.setY(std::stof(ettOrd));

            tempUVs.push_back(tempUV);
            continue;
        }
        if (ettOrd == "vn")
        {
            QVector3D tempNormal;
            sStream >> ettOrd;
            tempNormal.setX(std::stof(ettOrd));
            sStream >> ettOrd;
            tempNormal.setY(std::stof(ettOrd));
            sStream >> ettOrd;
            tempNormal.setZ(std::stof(ettOrd));

            tempRGB.push_back(tempNormal);
            continue;
        }
        if (ettOrd == "f")
        {
            int index, normal, uv;

            for (int i = 0; i <3 ; i++)
            {
                sStream >> ettOrd;
                std::stringstream tempOrd(ettOrd);
                std::string segment;
                std::vector<std::string> segmentArray;

                while(std::getline(tempOrd, segment, '/' ))
                {
                    segmentArray.push_back(segment);
                }
                index = std::stoi(segmentArray[0]);
                if (segmentArray[1] != "")
                    uv = std::stoi(segmentArray[1]);
                else
                {
                    uv = 0;
                }
                normal = std::stoi(segmentArray[2]);

                --index;
                --uv;
                --normal;

                if (uv > -1)
                {
                    Vertex tempVert(tempXYZ[index], tempRGB[normal], tempUVs[uv]);
                    mVertices.push_back(tempVert);
                }
                else
                {
                    Vertex tempVert(tempXYZ[index], tempRGB[normal], QVector2D(0.f,0.f));
                    mVertices.push_back(tempVert);
                }
                mIndices.push_back(tempIndex++);
            }
            continue;
        }
    }
    fileIn.close();
}

void ObjMesh::draw()
{
    glBindVertexArray( mVAO );
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
