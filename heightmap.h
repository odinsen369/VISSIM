#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <string>
#include <QImage>
#include <QString>
#include <QVector3D>
#include <vector>
#include "graphmaker.h"
#include "vertex.h"
#include "visualobject.h"

class HeightMap : public VisualObject
{
public:
    HeightMap(QString fuleName, float heightMultiplier=1, float scalePerStep=1, int pixelsPerStep=1);
    std::vector<Vertex> CreateArraysFromHeightmap(const QString &filepath, float multiplier, float scalePerStep, int pixelsPerStep);

    static UV CalculateUV(float xMin, float xMax, float yMin, float yMax, float x, float y);
    void init(GLint matrixUniform) override;
    void draw() override;
    QImage mImg;
    float mScalePerStep;
    float mHeightMultiplier;
    int mPixelsPerStep;
    QVector3D mCurrentNormal;


    float GetHeight(float x, float y);
    QVector3D GetNormal(float x, float y);
};

#endif // HEIGHTMAP_H
