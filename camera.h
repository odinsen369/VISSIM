#ifndef CAMERA_H
#define CAMERA_H

#include <QOpenGLFunctions_4_1_Core>
#include <QVector3D>
#include <QMatrix4x4>
#include "interactiveobject.h"

class Camera : public QOpenGLFunctions_4_1_Core
{
private:
    GLint mPmatrixUniform;
    GLint mVmatrixUniform;
    QVector3D mEye;

    InteractiveObject* mTarget{nullptr};
    QVector3D mCameraPos;
    QVector3D mTargetPos;
    QVector3D mDirection;

    float distanceToTarget{15.f};
    float mZoomSensitivity = 0.1f;
    float movementSensitivity = 0.7f;

public:
    Camera();
    QVector3D GetPosition();
      void init(GLint pMatrixUniform, GLint vMatrixUniform);
      void perspective(int degrees, double aspect, double nearplane, double farplane);
      void lookAt(const QVector3D& eye, const QVector3D& at, const QVector3D& up);
      void update(GLint pMat, GLint vMat);
      void lookAt(const QVector3D &at);
      void translate(float dx, float dy, float dz);
      void Tick(float deltaTime);
      void setPosition(const QVector3D &newPos);
      void Zoom(float diff);
      void followMouse(float diffX, float diffY);

      QVector3D getForward();
      QVector3D getUp();
      QVector3D getRight();

      void setTarget(InteractiveObject *target);
      QMatrix4x4 mPmatrix{};
      QMatrix4x4 mVmatrix{};

      QVector3D oldPosition;
};

#endif // CAMERA_H

