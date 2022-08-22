#include "camera.h"

QVector3D Camera::GetPosition()
{
    return mEye;
}
Camera::Camera() : mEye{0,-5,3}
{
    //mPmatrix.setToIdentity();
    //mVmatrix.setToIdentity();
//    mEye.setZ((7.f/6.f)*pow(-10,3) - 9*pow(-10,2) + (131.f/6.f)*-10 -12);
//    mEye.setX(-10);
}

void Camera::init(GLint pMatrixUniform, GLint vMatrixUniform)
{
    mPmatrix.setToIdentity();
    mVmatrix.setToIdentity();
    mPmatrixUniform = pMatrixUniform;
    mVmatrixUniform = vMatrixUniform;
}

void Camera::perspective(int degrees, double aspect, double nearplane, double farplane)
{
    mPmatrix.perspective(degrees, aspect, nearplane, farplane);
}

void Camera::lookAt(const QVector3D &eye, const QVector3D &at, const QVector3D &up)
{
    mVmatrix.lookAt(eye, at, up);
}

void Camera::lookAt(const QVector3D &at)
{
    mVmatrix.setToIdentity();
    mVmatrix.lookAt(mEye, at, QVector3D{0, 0, 1});
}

void Camera::translate(float dx, float dy, float dz)
{
    mVmatrix.translate(-dx, -dy, -dz);
}

void Camera::Tick(float deltaTime)
{
    QVector3D dir;

    mTarget != nullptr ? mTargetPos = mTarget->getPosition() : mTargetPos = QVector3D{0,0,0};
    dir = mEye - oldPosition;
    dir.normalize();

    mEye = dir * distanceToTarget + mTargetPos;

    if(mTarget != nullptr)
    {
        //        std::cout << "x: " << targetPos2.x() << std::endl;
        lookAt(mTargetPos/* * distanceToTarget*/);
    }
    else
    {
        lookAt(QVector3D{0, 0, 0});
    }

    oldPosition = mTargetPos;
}

void Camera::setPosition(const QVector3D &newPos)
{
    mEye = newPos;
}

QVector3D Camera::getForward()
{
//    QVector4D temp = mPmatrix.column(0);
//    temp.normalize();
//    return QVector3D{temp.x(), temp.y(), temp.z()};

    auto targetPos = mTarget != nullptr ? mTarget->getPosition() : QVector3D{0, 0, 0};

    QVector3D out = targetPos - mEye;

    out.normalize();

    //std::cout << "X: " << out.x() << "y: " << out.y() << "Z: " << out.z() << std::endl;
    return out;
}

QVector3D Camera::getUp()
{
    QVector3D out = QVector3D::crossProduct(getRight(), getForward());
    out.normalize();
    return out;
}

QVector3D Camera::getRight()
{
    QVector3D out = QVector3D::crossProduct(getForward(), {0,0,1});
    out.normalize();
    return out;
}

void Camera::setTarget(InteractiveObject *target)
{
    mTarget = target;
}

void Camera::update(GLint pMat, GLint vMat)
{
    initializeOpenGLFunctions();
    glUniformMatrix4fv( pMat, 1, GL_FALSE, mPmatrix.constData());
    glUniformMatrix4fv( vMat, 1, GL_FALSE, mVmatrix.constData());
}

void Camera::Zoom(float diff)
{
    diff = std::clamp(diff, -10.f, 10.f) * mZoomSensitivity;
    distanceToTarget = std::clamp(distanceToTarget - diff, 2.f, 40.f);
}

void Camera::followMouse(float diffX, float diffY)
{
    QMatrix4x4 rotation;

        rotation.setToIdentity();
        QVector3D around;
        if (mTarget == nullptr) {
            around = {0, 0, 0};
        } else {
            around = mTarget->getPosition();
        }

        QVector3D oldPosition = mEye;
        QVector3D oldRight = getRight();

        rotation.translate(around);
        rotation.rotate(diffY, getRight());
        rotation.rotate(diffX * getUp().z(), getUp());
        rotation.translate(-around);

        mEye = rotation.map(mEye);

        if (QVector3D::dotProduct(getRight(), oldRight) < 0)
            mEye = oldPosition;
        //std::cout << "X: " << Right().x() << "y: " << Right().y() << "Z: " << Right().z() << std::endl;

        mVmatrix.setToIdentity();

        mVmatrix.lookAt(mEye, around, QVector3D{0, 0, 1});
}
