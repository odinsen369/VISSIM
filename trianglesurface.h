#ifndef TRIANGLESURFACE_H
#define TRIANGLESURFACE_H

#include "visualobject.h"
#include <math.h>
#include "vertex.h"
//#include "vector3d.h"

class TriangleSurface : public VisualObject
{
public:
   TriangleSurface();
   TriangleSurface(std::string filnavn);
   ~TriangleSurface() override;
   void writeFile(std::string filnavn);
   void readFile(std::string filnavn);
   void init(GLint matrixUniform) override;
   void draw() override;
   void construct(double (f)(double,double));
   void solveIntegral(double(f)(double,double));
   static double funksjon(double x,double y);
   static double titty(double x,double y);
   static double ob2(double x,double y);
};

#endif // TRIANGLESURFACE_H
