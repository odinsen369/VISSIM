#ifndef MINSTEKVADRATER_H
#define MINSTEKVADRATER_H

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include "vertex.h"
#include "visualobject.h"

class MinsteKvadrater : public VisualObject
{
public:
    MinsteKvadrater();
    ~MinsteKvadrater() override;
    void construct(/*double (f)(double, double)*/);
//    static double funksjon(double x, double y);
    void init(GLint matrixUniform) override;
    void draw() override;
    std::vector <Vertex> punkter;
    friend class NPC;
};
#endif // MINSTEKVADRATER_H
