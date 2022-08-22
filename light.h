#ifndef LIGHT_H
#define LIGHT_H

#include "visualobject.h"

class Light : public VisualObject
{
public:
    Light();
    void init(GLint matrixUniform) override;
    void draw() override;

    GLfloat mAmbientStrenght{0.3f};
    QVector3D mAmbientColor{0.3f, 0.3f, 0.3f};  //Grey

    GLfloat mLightStrenght{0.7f};
    QVector3D mLightColor{0.9f, 0.9f, 0.9f};    //Yellow

    QVector3D mOrbitPoint {5,0,10};
    float mSpecularStrenght{0.9f};
    int mSpecularExponent{4};

    void orbit(float dt);
};

#endif // LIGHT_H
