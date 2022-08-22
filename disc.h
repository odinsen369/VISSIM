#ifndef DISC_H
#define DISC_H

#include "interactiveobject.h"

class Disc : public InteractiveObject
{
public:
   Disc();
   Disc(std::string filnavn);
   ~Disc() override;
   void readFile(std::string filnavn);
   void writeFile(std::string filnavn);
   void init(GLint matrixUniform) override;
   void draw() override;
   void construct();
   void move(float dt) override;
   QVector3D mVelocity{0.1,0,0};
protected:
   std::vector<GLuint> mIndices;   	// Til indeksering
   GLuint mIBO{0};                 		// for glDrawElements()

   float mRadius{0.5};
};


#endif // DISC_H
