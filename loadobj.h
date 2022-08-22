#ifndef LOADOBJ_H
#define LOADOBJ_H

#include "vector"
#include "visualobject.h"
#include <fstream>
#include <iostream>
#include <string>

class LoadOBJ : public VisualObject
{
public:
    LoadOBJ();
    ~LoadOBJ();
    void init(GLint matrixUniform);
    void draw();
    void readFile(std::string filnavn);
};

#endif // LOADOBJ_H
