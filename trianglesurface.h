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
   void triangulate();
   static double funksjon(double x,double y);
   static double titty(double x,double y);
   static double ob2(double x,double y);

   std::vector<Vertex> trianguler;

   float xMin{99999999.f};
   float xMax{0};
   float yMin{99999999.f};
   float yMax{0};
   float oldHeight{660.f};
   int width{0};
   int height{0};
};

#endif // TRIANGLESURFACE_H
