#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include "visualobject.h"

class BezierCurve : public VisualObject
{
public:
    BezierCurve();
    ~BezierCurve();
    void init(GLint matrixUniform) override;
    void draw() override;
    void makebezier(Vertex v1, Vertex v2,Vertex v3, Vertex v4);
    Vertex CalculateBezier(Vertex v1,Vertex v2,Vertex v3,Vertex v4, float x);
};

#endif // BEZIERCURVE_H
