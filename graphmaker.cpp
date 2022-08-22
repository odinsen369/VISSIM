#include "graphmaker.h"

GraphMaker::GraphMaker()
{

}

GraphMaker::~GraphMaker() {}

float GraphMaker::my2DEquation(float x)
{
    return sin(x-(pow(x, 2)*0.2));
}

UV GraphMaker::CalculateUV(float xMin, float xMax, float yMin, float yMax, float x, float y)
{
    UV out;

    float xRange = xMax - xMin;
    float xPos = x - xMin;
    out.u = xPos/xRange;
    float yRange = yMax - yMin;
    float yPos = y - yMin;
    out.v = yPos/yRange;

    return out;
}

std::vector<Vertex> GraphMaker::Create2DGraph(float f(float), float size, float subdivisions)
{
    std::vector<Vertex> mVertices;
    subdivisions = size / subdivisions;
    float xmin=-size, xmax=size, h=subdivisions; // The size to draw
    for (auto x=xmin; x<xmax; x+=h)

        {
            mVertices.push_back(Vertex{x, 0, 0, x, 0.2, 0.2});

            mVertices.push_back(Vertex{x+h, 0, 0, x, 0.2, 0.2});

            mVertices.push_back(Vertex{x, 0, f(x), abs(f(x))*0.6f, abs(f(x)), abs(f(x))});

            mVertices.push_back(Vertex{x, 0, f(x), abs(f(x))*0.6f, abs(f(x)), abs(f(x))});

            mVertices.push_back(Vertex{x+h, 0, 0, x, 0.2, 0.2});

            mVertices.push_back(Vertex{x+h, 0, f(x+h), abs(f(x+h))*0.6f, abs(f(x+h)), abs(f(x+h))});

        }
    return mVertices;
}

std::vector<Vertex> GraphMaker::Create3DGraph(float f(float, float), float size, float subdivisions)
{
    std::vector<Vertex> mVertices;
    subdivisions = size / subdivisions;


    float xmin=-size, xmax=size, ymin=-size, ymax=size, h=subdivisions; // The size to draw
    for (auto x=xmin; x<xmax; x+=h)
        for (auto y=ymin; y<ymax; y+=h)
        {
            float xSquared = x * x;
            float ySquared = y * y;
            float combined = xSquared + ySquared;
            float lengthFromOrigin = sqrt(combined);
            if (lengthFromOrigin > size) {
                continue;
            }


            float z = f(x,y);
            UV uv = CalculateUV(xmin, xmax, ymin, ymax, x, y);
            mVertices.push_back(Vertex{x,y,z,0,0,1,uv.u, uv.v});

            z = f(x+h,y);
            uv = CalculateUV(xmin, xmax, ymin, ymax, x+h, y);
            mVertices.push_back(Vertex{x+h,y,z,0,0,1,uv.u, uv.v});

            z = f(x,y+h);
            uv = CalculateUV(xmin, xmax, ymin, ymax, x, y+h);
            mVertices.push_back(Vertex{x,y+h,z,0,0,1,uv.u, uv.v});

            uv = CalculateUV(xmin, xmax, ymin, ymax, x, y+h);
            mVertices.push_back(Vertex{x,y+h,z,0,0,1,uv.u, uv.v});

            z = f(x+h,y);
            uv = CalculateUV(xmin, xmax, ymin, ymax, x+h, y);
            mVertices.push_back(Vertex{x+h,y,z,0,0,1,uv.u, uv.v});

            z = f(x+h,y+h);
            uv = CalculateUV(xmin, xmax, ymin, ymax, x+h, y+h);
            mVertices.push_back(Vertex{x+h,y+h,z,0,0,1,uv.u, uv.v});
        }


    return mVertices;
}

void GraphMaker::writeFile(std::string filnavn)
{
    std::ofstream fil("../3Dprog22konte/"+filnavn);
    if(fil.is_open())
    {
        fil << mVertices.size() << "\n";
        for(auto i = 0; i < mVertices.size(); i++)
            fil << mVertices[i] << "\n";

        //sender antall vertices og alle vertexene inn i oblig1 tekstfila
        fil.close();
    }
}

void GraphMaker::readFile(std::string filnavn) {
    std::ifstream fil;
    fil.open("../3Dprog22konte/"+filnavn); //åpner oblig1.txt
    if (fil.is_open()) {
        int count;
        Vertex vertex;
        fil >> count;
        mVertices.reserve(count);
        for (int i=0; i<count; i++) {
             fil >> vertex;
             mVertices.push_back(vertex); //sender vertexene fra tekstfila inn i mVertices
        }
        fil.close();
    }
    mMatrix.setToIdentity();
}

std::vector<Vertex> GraphMaker::makeBezier(Vertex v1, Vertex v2,Vertex v3, Vertex v4)
{
    std::vector<Vertex> mVertices;
    mVertices.push_back(v4);
    mVertices.push_back(v3);
    mVertices.push_back(v2);
    mVertices.push_back(v1);
    Vertex temp{0,0,0,0,0};
    for (float x = 0; x<1; x+=0.01f)
    {
        temp = CalculateBezier(v1,v2,v3,v4,x); //sender inn punkter
        mVertices.push_back(temp);
    }
    return mVertices;
}

Vertex GraphMaker::CalculateBezier(Vertex v1,Vertex v2,Vertex v3,Vertex v4, float t)
{
    Vertex temp{0,0,1,0,1};

    temp.setX(
                        ( pow((1-t), 3) * v1.getX()         )
                    +   ( pow((1-t), 2) * 3 * t * v2.getX() )   //putter punkter inn i funksjonen for kubisk bezierkurve
                    +   ( (1 - t) * 3 * t * t *  v3.getX()  )
                    +   ( t * t * t * v4.getX()             )
                    );
    temp.setZ(
                        ( pow((1-t), 3) * v1.getZ()         )
                    +   ( pow((1-t), 2) * 3 * t * v2.getZ() )
                    +   ( (1 - t) * 3 * t * t *  v3.getZ()  )
                    +   ( t * t * t * v4.getZ()             )
                    );
    return temp;
}
