#ifndef TERRAIN_H
#define TERRAIN_H

#include "visualobject.h"

class terrain : public VisualObject
{
public:
    terrain();
    ~terrain();
    terrain(std::string filnavn);
    void init(GLint matrixUniform) override;
    void draw() override;
    void readFile(std::string filnavn);
    void triangulate();
    void buildMesh();

    std::vector<Vertex> trianguler;
    std::vector<Vertex> trianguler2;

    float xMin{99999999.f};
    float xMax{0};
    float yMin{99999999.f};
    float yMax{0};
    float oldHeight{660.f};
    int width{0};
    int height{0};
    float rotation{0};
};

#endif // TERRAIN_H
