#ifndef INTERACTIVEOBJECT_H
#define INTERACTIVEOBJECT_H
#include "visualobject.h"

class InteractiveObject : public VisualObject
{
public:
    InteractiveObject();
    ~InteractiveObject() override;
    void init(GLint matrixUniform) override;
    void draw() override;
    void move(float dx, float dy, float dz) override;

    void setPosition(float x, float y, float z);
    void setPosition(QVector3D pos);
    void setScale(float scale);
    void rotate(float degrees, QVector3D vector = {0,0,1});

    void setHeight(float z);
    void setDrawMethod(DrawMethod method) override;

    void floor(float height);

    void push(QVector3D dir, float force);
    QVector3D getPosition() override;
    bool hasGravity = false;
    QVector3D mGravity{0,0,-0.1f};

    class HeightMap* hmfloor{nullptr};

    void Tick(float deltaTime);

    QVector3D getForward();

    void move(QVector3D vec);
protected:
    float mx, my, mz; // posisjon
    void updateMatrix();
    float floorLevel{0.f};
    bool mIsOnGround{false};
    float mFloorLevel{0.f};
    QVector3D mSpeed{0,0,0};
};

#endif // INTERACTIVEOBJECT_H
