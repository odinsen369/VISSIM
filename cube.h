#ifndef CUBE_H
#define CUBE_H

#include "visualobject.h"

class Cube : public VisualObject
{
public:
    Cube();
    Cube(float x, float y, float z);
    ~Cube();
    void init(GLint matrixUniform) override;
    void draw() override;
    static std::vector<Vertex> createcube();
    static void MakeSquare(std::vector<Vertex>& mVertices, QVector3D direction);
};

#endif // CUBE_H
