#ifndef MESHGENERATOR_H
#define MESHGENERATOR_H
#include <vector>
#include "vertex.h"
#include "graphmaker.h"
#include "cube.h"
#include "visualobject.h"
#include "octahedronball.h"

class MeshGenerator :  public GraphMaker, public Cube, public VisualObject, public OctahedronBall
{
public:
    MeshGenerator();
    static std::vector<Vertex> Mesh2DGraph(float(f)(float), float size = 10, float subdivisions = 80){
        return Create2DGraph(f, size, subdivisions);}
    static std::vector<Vertex> Mesh3DGraph(float(f)(float, float), float size = 10, float subdivisions = 80){
        return Create3DGraph(f, size, subdivisions);}
    static std::vector<Vertex> BezierMake(Vertex v1, Vertex v2, Vertex v3, Vertex v4){
        return makeBezier(v1,v2,v3,v4);}
    static std::vector<Vertex> CubeMaker(){
        return createcube();}
    static std::vector<Vertex> GenerateOktahedron(int divisions){return oktaederUnitBall(divisions);}

    static std::vector<Vertex> Triangle();
    static std::vector<Vertex> Plane(float size);
    static std::vector<Vertex> Dot();
    static std::vector<Vertex> createColoredCube(std::string color);
    static void MakeColoredSquare(std::vector<Vertex>& mVertices, QVector3D direction, std::string color);
    static std::vector<Vertex> Fence();
    static std::vector<Vertex> xyz(float size);

};

#endif // MESHGENERATOR_H
