#ifndef VERTEX_H
#define VERTEX_H
#include <iostream>
#include <fstream>
#include <QVector3D>

struct Vertex0{
    float x;
    float y;
    float z;
    float r;
    float g;
    float b;
};

class Vertex {
   //! Overloaded ostream operator which writes all vertex data on an open textfile stream
    friend std::ostream& operator<< (std::ostream&, const Vertex&);

      //! Overloaded ostream operator which reads all vertex data from an open textfile stream
      friend std::istream& operator>> (std::istream&, Vertex&);

   private:
//      float m_xyz[3];       	// position i 3D
//      float m_normal[3];    // normal in 3D or rgb colors
//      float m_st[2];        	// texture coordinates if used

   public:
    float m_xyz[3];       	// position i 3D
    float m_normal[3];    // normal in 3D or rgb colors
    float m_st[2];        	// texture coordinates if used
    int m_neighbour[1];

      Vertex();
      Vertex(float x, float y, float z, float r, float g, float b);
      Vertex(float x, float y, float z);
      Vertex(float x, float y, float r, float g, float b);
      Vertex(QVector3D xyz, float r, float g, float b);
      Vertex(QVector3D a, QVector3D b, QVector2D c);
      Vertex(float x, float y, float z, float r, float g, float b, float s, float t);
      Vertex(QVector3D xyz, QVector3D rgb, float u, float v);
      Vertex(QVector3D xyz, QVector3D rgb);

      void setX(float px);
      void setY(float py);
      void setZ(float pz);
      void divide();
      void setNeighbour(int a);

      float getX();
      float getY();
      float getZ();
};

#endif // VERTEX_H
