#include <meshgenerator.h>

std::vector<Vertex> MeshGenerator::Plane(float size)
{
    std::vector<Vertex> vertOut;
        vertOut.push_back(Vertex{-size,size,0,1,0,0,0,1});
        vertOut.push_back(Vertex{size,-size,0,1,0,0,1,0});
        vertOut.push_back(Vertex{size,size,0,0,1,1,1,1});

        vertOut.push_back(Vertex{-size,size,0,0,0,1,0,1});
        vertOut.push_back(Vertex{-size,-size,0,0,0,1,0,0});
        vertOut.push_back(Vertex{size,-size,0,0,0,1,1,0});
        return vertOut;
}

std::vector<Vertex> MeshGenerator::Triangle()
{
    std::vector<Vertex> mVertices;

    // Positions            // Colors       //UV
    mVertices.push_back(Vertex{-0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  0.f, 0.f}); // Bottom Left
    mVertices.push_back(Vertex{0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,    1.0f, 0.f}); // Bottom Right
    mVertices.push_back(Vertex{0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.5f, 1.f}); // Top
    return mVertices;
}

std::vector<Vertex> MeshGenerator::xyz(float size)
{
    std::vector<Vertex> mVertices;
    mVertices.push_back(Vertex{0,0,0,1,0,0});
    mVertices.push_back(Vertex{size,0,0,1,0,0});
    mVertices.push_back(Vertex{0,0,0,0,1,0});
    mVertices.push_back(Vertex{0,size,0,0,1,0});
    mVertices.push_back(Vertex{0,0,0,0,0,1});
    mVertices.push_back(Vertex{0,0,size,0,0,1});
    return mVertices;
}
std::vector<Vertex> MeshGenerator::Dot()
{
    std::vector<Vertex> vertOut;
    vertOut.push_back(Vertex{0,0,0,1,0,0,0,1});
    vertOut.push_back(Vertex{0,0,0,1,0,0,1,0});
    vertOut.push_back(Vertex{0,0,0,0,1,1,1,1});
    return vertOut;
}

std::vector<Vertex> MeshGenerator::createColoredCube(std::string color)
{
    std::vector<Vertex> mVertices;
    mVertices.reserve(36);
    MakeColoredSquare(mVertices, {0,0,-1}, color);
    MakeColoredSquare(mVertices, {0,0,1},  color);
    MakeColoredSquare(mVertices, {0,1,0},  color);
    MakeColoredSquare(mVertices, {0,-1,0}, color);
    MakeColoredSquare(mVertices, {-1,0,0}, color);
    MakeColoredSquare(mVertices, {1,0,0},  color);
    return mVertices;
}

void MeshGenerator::MakeColoredSquare(std::vector<Vertex>& mVertices, QVector3D direction, std::string color)
{
    auto center = direction*(1/2);

    auto temp = center;

    temp.setX(abs(abs(direction.x())-1));
    temp.setY(abs(abs(direction.y())-1));
    temp.setZ(abs(abs(direction.z())-1));

    auto corner1 = temp;
    corner1.setX(corner1.x() * -1);

    auto corner2 = temp;
    corner2.setY(corner2.y() * -1);

    auto corner3 = temp;
    corner3.setZ(corner3.z() * -1);

    temp.setX(abs(abs(direction.x())-1)*-1);
    temp.setY(abs(abs(direction.y())-1)*-1);
    temp.setZ(abs(abs(direction.z())-1)*-1);

    auto corner4 = temp;
    corner4.setX(corner4.x() * -1);

    auto corner5 = temp;
    corner5.setY(corner5.y() * -1);

    auto corner6 = temp;
    corner6.setZ(corner6.z() * -1);

    corner1 = corner1 + direction;
    corner2 = corner2 + direction;
    corner3 = corner3 + direction;
    corner4 = corner4 + direction;
    corner5 = corner5 + direction;
    corner6 = corner6 + direction;

    if(color == "red")
    {
        mVertices.push_back(Vertex{corner1/5, 1, 0, 0});
        mVertices.push_back(Vertex{corner2/5, 1, 0, 0});
        mVertices.push_back(Vertex{corner3/5, 1, 0, 0});
        mVertices.push_back(Vertex{corner4/5, 1, 0, 0});
        mVertices.push_back(Vertex{corner5/5, 1, 0, 0});
        mVertices.push_back(Vertex{corner6/5, 1, 0, 0});
    }
    if(color == "blue")
    {
        mVertices.push_back(Vertex{corner1/5, 0, 0, 1});
        mVertices.push_back(Vertex{corner2/5, 0, 0, 1});
        mVertices.push_back(Vertex{corner3/5, 0, 0, 1});
        mVertices.push_back(Vertex{corner4/5, 0, 0, 1});
        mVertices.push_back(Vertex{corner5/5, 0, 0, 1});
        mVertices.push_back(Vertex{corner6/5, 0, 0, 1});
    }
}

std::vector<Vertex> MeshGenerator::Fence()
{
    std::vector<Vertex> mVertices;
    mVertices.push_back(Vertex{0, 0, 0, 1, 1, 1});
    mVertices.push_back(Vertex{1, 0, 0, 1, 1, 1});
    mVertices.push_back(Vertex{0, 0, 2, 1, 1, 1}); //front square
    mVertices.push_back(Vertex{0, 0, 2, 1, 1, 1});
    mVertices.push_back(Vertex{1, 0, 0, 1, 1, 1});
    mVertices.push_back(Vertex{1, 0, 2, 1, 1, 1});

    mVertices.push_back(Vertex{1, 0, 2, 1, 1, 1});
    mVertices.push_back(Vertex{1, 0, 0, 1, 1, 1});
    mVertices.push_back(Vertex{1, 5, 0, 1, 1, 1}); //right rectangle
    mVertices.push_back(Vertex{1, 0, 2, 1, 1, 1});
    mVertices.push_back(Vertex{1, 5, 0, 1, 1, 1});
    mVertices.push_back(Vertex{1, 5, 2, 1, 1, 1});

    mVertices.push_back(Vertex{0, 5, 0, 1, 1, 1});
    mVertices.push_back(Vertex{1, 5, 0, 1, 1, 1});
    mVertices.push_back(Vertex{0, 5, 2, 1, 1, 1}); //back square
    mVertices.push_back(Vertex{0, 5, 2, 1, 1, 1});
    mVertices.push_back(Vertex{1, 5, 0, 1, 1, 1});
    mVertices.push_back(Vertex{1, 5, 2, 1, 1, 1});

    mVertices.push_back(Vertex{0, 0, 2, 1, 1, 1});
    mVertices.push_back(Vertex{0, 0, 0, 1, 1, 1});
    mVertices.push_back(Vertex{0, 5, 0, 1, 1, 1}); //left rectangle
    mVertices.push_back(Vertex{0, 0, 2, 1, 1, 1});
    mVertices.push_back(Vertex{0, 5, 0, 1, 1, 1});
    mVertices.push_back(Vertex{0, 5, 2, 1, 1, 1});

    mVertices.push_back(Vertex{0, 5, 2, 1, 1, 1});
    mVertices.push_back(Vertex{1, 5, 2, 1, 1, 1});
    mVertices.push_back(Vertex{1, 0, 2, 1, 1, 1}); //top rectangle
    mVertices.push_back(Vertex{1, 0, 2, 1, 1, 1});
    mVertices.push_back(Vertex{0, 0, 2, 1, 1, 1});
    mVertices.push_back(Vertex{0, 5, 2, 1, 1, 1});

    mVertices.push_back(Vertex{0, 5, 0, 1, 1, 1});
    mVertices.push_back(Vertex{1, 5, 0, 1, 1, 1});
    mVertices.push_back(Vertex{1, 0, 0, 1, 1, 1}); //bottom rectangle
    mVertices.push_back(Vertex{1, 0, 0, 1, 1, 1});
    mVertices.push_back(Vertex{0, 0, 0, 1, 1, 1});
    mVertices.push_back(Vertex{0, 5, 0, 1, 1, 1});
    return mVertices;
}
