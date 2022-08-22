#ifndef OBJMESH_H
#define OBJMESH_H

#include "visualobject.h"

class ObjMesh : public VisualObject
{
public:
    ObjMesh(std::string filnavn);
    ~ObjMesh() override;

    virtual void draw() override;
    virtual void init(GLint matrixUniform) override;

private:
    void readFile(std::string filnavn);
};

#endif // OBJMESH_H
