#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include "vertex.h"

enum DrawMethod{
    Triangles = GL_TRIANGLES, LineLoop = GL_LINE_LOOP, LineStrip = GL_LINE_STRIP, Lines = GL_LINES, Point = GL_POINT
};

struct ShaderInfo{
    class Shader* Shader            {nullptr};
    GLint* MatrixUniform            {nullptr};

    GLint* ViewMatrixUniform        {nullptr};
    GLint* ProjectionMatrixUniform  {nullptr};

    GLint* TextureUniform           {nullptr};

    GLint TextureID {0};
};

class VisualObject : public QOpenGLFunctions_4_1_Core {
public:
   VisualObject();
   ~VisualObject();
   virtual void init(GLint matrixUniform)=0;
   virtual void draw()=0;
   //virtual void drawdot()=0;
   virtual void move(){};
   virtual void move(float x, float y, float z) {  };
   virtual void move(float x, float y) {  };
   virtual void move(float dt) {};
   virtual void setDrawMethod(DrawMethod a){};
   virtual QVector3D getPosition();
   void setVertices(std::vector<Vertex> vertices);

//   virtual void setVertices(std::vector<Vertex> vertices);

   QMatrix4x4 mMatrix;

   int mDrawMethod = GL_TRIANGLES;

   ShaderInfo mShaderInfo;
      void setupShader(ShaderInfo shaderInfo) {mShaderInfo = shaderInfo;}

    QVector3D Barycentric(QVector3D object, QVector3D P1, QVector3D P2, QVector3D P3);
    bool isOverlappingTriangle(QVector3D baryc, QVector3D P1, QVector3D P2, QVector3D P3);
    float GetBarycentricHeight(QVector3D baryc, QVector3D P1, QVector3D P2, QVector3D P3);

protected:
   std::vector<Vertex> mVertices;
   std::vector<int> mIndices;
   GLuint mVAO{0};
   GLuint mVBO{0};
   GLuint mEAB{0};
   GLint mMatrixUniform{0};

   QMatrix4x4 mPosition;
   QMatrix4x4 mRotation;
   QMatrix4x4 mScale;

};
#endif // VISUALOBJECT_H
