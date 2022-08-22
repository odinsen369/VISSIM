#ifndef GRAPHMAKER_H
#define GRAPHMAKER_H
#include <vector>
#include "visualobject.h"

struct UV {
    float u, v;
};

class GraphMaker : VisualObject
{
public:
    GraphMaker();
    ~GraphMaker();
    void writeFile(std::string filnavn);
    void readFile(std::string filnavn);
    static std::vector<Vertex> Create2DGraph(float(f)(float), float size, float subdivisions);
    static std::vector<Vertex> Create3DGraph(float (f)(float, float), float size, float subdivisions);
    static Vertex CalculateBezier(Vertex v1,Vertex v2,Vertex v3,Vertex v4, float t);
    static std::vector<Vertex> makeBezier(Vertex v1,Vertex v2,Vertex v3,Vertex v4);
    static float my2DEquation(float x);
    static UV CalculateUV(float xMin, float xMax, float yMin, float yMax, float x, float y);
};

#endif // GRAPHMAKER_H
