#include "visualobject.h"

VisualObject::VisualObject() {  }
VisualObject::~VisualObject() {
   glDeleteVertexArrays( 1, &mVAO );
   glDeleteBuffers( 1, &mVBO );
}

QVector3D VisualObject::getPosition()
{
    std::cout << "VisualObject::getPosition()";
    QVector4D position = mPosition.column(3);
    return QVector3D{position.x(), position.y(), position.z()};
}

void VisualObject::setVertices(std::vector<Vertex> vertices)
{
    mVertices = vertices;
}

float VisualObject::GetBarycentricHeight(QVector3D baryc, QVector3D P1, QVector3D P2, QVector3D P3)
{
    //auto baryc = Barycentric(P1, P2, P3);
    return P1.z() * baryc.x() + P2.z() * baryc.y() + P3.z() * baryc.z();
}

QVector3D VisualObject::Barycentric(QVector3D object, QVector3D P1, QVector3D P2, QVector3D P3)
{
    P1.setZ(0);
    P2.setZ(0);
    P3.setZ(0);

    QVector3D P12 = (P2-P1);
    QVector3D P13 = (P3-P1);

    QVector3D n = QVector3D::crossProduct(P12, P13);
    float areal_123 = n.length();

    QVector3D baryc;

    // u
    QVector3D p = P2 - object;
    QVector3D q = P3 - object;
    n = QVector3D::crossProduct(p,q);
    baryc.setX(n.z()/areal_123);

    // v
    p = P3 - object;
    q = P1 - object;
    n = QVector3D::crossProduct(p,q);
    baryc.setY(n.z()/areal_123);

    // w
    p = P1 - object;
    q = P2 - object;
    n = QVector3D::crossProduct(p,q);
    baryc.setZ(n.z()/areal_123);

    return baryc;
}

bool VisualObject::isOverlappingTriangle(QVector3D baryc, QVector3D P1, QVector3D P2, QVector3D P3)
{
    //auto baryc = Barycentric(P1, P2, P3);
    return baryc.x() >= 0.f && baryc.y() >= 0.f && baryc.z() >= 0.f;
}

