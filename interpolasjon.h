#ifndef INTERPOLASJON_H
#define INTERPOLASJON_H

#include "visualobject.h"

class Interpolasjon : public VisualObject
{
public:
    Interpolasjon();
    ~Interpolasjon();
    void construct();
    //static double funksjon(double x, double y);
    void init(GLint matrixUniform) override;
    void draw() override;
};

#endif // INTERPOLASJON_H
