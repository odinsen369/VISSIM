#include "vertex.h"

Vertex::Vertex()
{

}

Vertex::Vertex(float x, float y, float z, float r, float g, float b)
{
    m_xyz[0] = x;
    m_xyz[1] = y;
    m_xyz[2] = z;
    m_normal[0] = r;
    m_normal[1] = g;
    m_normal[2] = b;
    m_st[0]=0.f;
    m_st[1]=0.f;
}

Vertex::Vertex(float x, float y, float z)
{
    m_xyz[0] = x;
    m_xyz[1] = y;
    m_xyz[2] = z;
}

Vertex::Vertex(float x, float y, float r, float g, float b)
    :m_xyz{x,y,0}, m_normal{r,g,b}
{

}

Vertex::Vertex(QVector3D xyz, float r, float g, float b)
    :m_xyz{xyz.x(), xyz.y(), xyz.z()}, m_normal{r,g,b}
{

}

Vertex::Vertex(QVector3D a, QVector3D b, QVector2D c)
{
    m_xyz[0] = a[0];
    m_xyz[1] = a[1];
    m_xyz[2] = a[2];
    m_normal[0] = b[0];
    m_normal[1] = b[1];
    m_normal[2] = b[2];
    m_st[0] = c[0];
    m_st[1] = c[1];
}

Vertex::Vertex(float x, float y, float z, float r, float g, float b, float s, float t)
{
    m_xyz[0] = x;
    m_xyz[1] = y;
    m_xyz[2] = z;
    m_normal[0] = r;
    m_normal[1] = g;
    m_normal[2] = b;
    m_st[0]=s;
    m_st[1]=t;
}

Vertex::Vertex(QVector3D xyz, QVector3D rgb, float u, float v)
 :m_xyz{xyz.x(), xyz.y(), xyz.z()}, m_normal{rgb.x(), rgb.y(), rgb.z()}, m_st{u, v}
{

}

Vertex::Vertex(QVector3D xyz, QVector3D rgb)
:m_xyz{xyz.x(), xyz.y(), xyz.z()}, m_normal{rgb.x(), rgb.y(), rgb.z()}
{

}
void Vertex::setX(float px)
{
    m_xyz[0] = px;
}

void Vertex::setY(float py)
{
    m_xyz[1] = py;
}

void Vertex::setZ(float pz)
{
    m_xyz[2] = pz;
}

void Vertex::divide()
{
    m_xyz[1] /= 10;
}

float Vertex::getX()
{
    return m_xyz[0];
}

float Vertex::getY()
{
    return m_xyz[1];
}

float Vertex::getZ()
{
    return m_xyz[2];
}

void Vertex::setNeighbour(int a)
{
    m_neighbour[0] = a;
}

QVector3D Vertex::getXYZ()
{
    return QVector3D{m_xyz[0], m_xyz[1], m_xyz[2]};
}

std::ostream& operator<< (std::ostream& os, const Vertex& v) {
  os << std::fixed;
  os << "(" << v.m_xyz[0] << ", " << v.m_xyz[1] << ", " << v.m_xyz[2] << ") ";
  os << "(" << v.m_normal[0] << ", " << v.m_normal[1] << ", " << v.m_normal[2] << ") ";
  os << "(" << v.m_st[0] << ", " << v.m_st[1] << ") ";
  return os;
}
std::istream& operator>> (std::istream& is, Vertex& v) {
// Trenger fire temporære variabler som kun skal lese inn parenteser og komma
  char dum, dum2, dum3, dum4;
  is >> dum >> v.m_xyz[0] >> dum2 >> v.m_xyz[1] >> dum3 >> v.m_xyz[2] >> dum4;
  is >> dum >> v.m_normal[0] >> dum2 >> v.m_normal[1] >> dum3 >> v.m_normal[2] >> dum4;
  is >> dum >> v.m_st[0] >> dum2 >> v.m_st[1] >> dum3;
  return is;
}
