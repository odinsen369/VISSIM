#include "interactiveobject.h"
#include "heightmap.h"

InteractiveObject::InteractiveObject() : mx{0.0f}, my{0.0f}, mz{0.0f}
{

}

InteractiveObject::~InteractiveObject()
{

}

void InteractiveObject::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;
    initializeOpenGLFunctions();

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glBufferData(GL_ARRAY_BUFFER,
                 mVertices.size()* sizeof (Vertex),
                 mVertices.data(),
                 GL_STATIC_DRAW
                 );

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(
                0,                  // attribute. No particular reason for 0, but must match layout(location = 0) in the vertex shader.
                3,                  // size / number of elements of data type
                GL_FLOAT,           // data type
                GL_FALSE,           // normalize data
                sizeof(Vertex),  // stride
                reinterpret_cast<GLvoid*>(0));          // array buffer offset
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof (Vertex),  reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
}

void InteractiveObject::draw()
{
    glBindVertexArray( mVAO );
//    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(mDrawMethod, 0, mVertices.size());
}

void InteractiveObject::move(float dx, float dy, float dz)
{
    mx += dx;
    my += dy;
    mz += dz;
    mPosition.translate(dx,dy,dz);
    updateMatrix();
}

void InteractiveObject::move(QVector3D vec)
{
    float dx,dy,dz;
    dx = vec.x();
    dy= vec.y();
    dz = vec.z();
    move(dx,dy,dz);
}

void InteractiveObject::setHeight(float z)
{
    mPosition.translate(0,0,z-mz);
    mz = z;
    updateMatrix();
}

void InteractiveObject::rotate(float degrees, QVector3D vector)
{
    QMatrix4x4 newRotation;
    newRotation.setToIdentity();
    newRotation.rotate(degrees, vector);
    mRotation = newRotation * mRotation;

    updateMatrix();
}

void InteractiveObject::setScale(float scale)
{
    QMatrix4x4 newScale;
    newScale.setToIdentity();
    newScale.scale({scale, scale, scale});
    mScale = newScale;

    updateMatrix();
}

void InteractiveObject::setPosition(float x, float y, float z)
{
    mPosition.translate(x-mx, y-my, z-mz);
    mx = x;
    my = y;
    mz = z;
    updateMatrix();
}

void InteractiveObject::setPosition(QVector3D pos)
{
    mPosition.translate(pos.x()-mx, pos.y()-my, pos.z()-mz);
    mx = pos.x();
    my = pos.y();
    mz = pos.z();
    updateMatrix();
}

void InteractiveObject::updateMatrix()
{
    mMatrix = mPosition * mRotation * mScale;
}

void InteractiveObject::setDrawMethod(DrawMethod method)
{
    mDrawMethod = method;
}

QVector3D InteractiveObject::getPosition()
{
//    std::cout << mx << " - " << my << " - " << mz << std::endl;
    return QVector3D{mx, my, mz};
}

void InteractiveObject::floor(float height)
{
    floorLevel = height;
}

void InteractiveObject::push(QVector3D dir, float force)
{
    mSpeed += dir.normalized() * force;
}

void InteractiveObject::Tick(float deltaTime)
{
    if (hasGravity)
    {
        mSpeed += mGravity;
        move(mSpeed * deltaTime);
        mSpeed *= 0.999;
    }
        if (hmfloor != nullptr) {
            float floorHeight = hmfloor->GetHeight(mx, my);
            if (mz < floorHeight + 1) {
                mIsOnGround = true;
                QVector3D floorNormal = hmfloor->GetNormal(mx, my);
                setHeight(floorHeight+1);
                mSpeed = mSpeed - 2 * (QVector3D::dotProduct(mSpeed, floorNormal)) * floorNormal * 0.9;
                //mVelocity.setZ(mVelocity.z() + fmaxf(floorHeight, 0));
            }
            else {
                mIsOnGround = false;
            }
        }
        else if (mz < mFloorLevel+1) {
            //setHeight(mFloorLevel+1);
            mSpeed.setZ(-mSpeed.z());
            mIsOnGround = true;
        }
        else {
            mIsOnGround = false;
        }
}

QVector3D InteractiveObject::getForward() {
    QVector3D forward {0,-0.1f,0};
    forward = mRotation * forward;
    return forward;
}

