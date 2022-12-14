#include "trianglesurface.h"
//#include <iostream>
#include <fstream>

TriangleSurface::TriangleSurface()
{
  construct(ob2); //lager txt fila
}

TriangleSurface::~TriangleSurface() { }

TriangleSurface::TriangleSurface(std::string filnavn)
{
   readFile(filnavn);
}

void TriangleSurface::writeFile(std::string filnavn)
{
    std::ofstream fil("../3Dprog22kontekonte/"+filnavn);
    if(fil.is_open())
    {
        fil << mVertices.size() << "\n";
        for(auto i = 0; i < mVertices.size(); i++)
            fil << mVertices[i] << "\n";

        //sender antall vertices og alle vertexene inn i oblig1 tekstfila
        fil.close();
    }
}

void TriangleSurface::triangulate()
{
    float h = 5.f;
    for (auto x=xMin; x<xMax; x+=h)
    {
        for (auto y=yMin; y<yMax; y+=h)
        {
            float z = 0;
            int amount = 0;
            for (int i = 0; i < trianguler.size(); i++)
            {
                if (abs(trianguler.at(i).getX()-x) < h && abs(trianguler.at(i).getY()-y) < h)
                {
                    z += trianguler.at(i).getZ();
                    amount += 1;
                }
            }
//            height += 1;
            z = z/amount;

            mVertices.push_back(Vertex{x+(h/2),y+(h/2),z+(z-oldHeight),x,y,z});
            mVertices.push_back(Vertex{x+(h/2)+h,y+(h/2),z,x,y,z});
            mVertices.push_back(Vertex{x+(h/2),y+(h/2)+h,z,x,y,z});
            mVertices.push_back(Vertex{x+(h/2),y+(h/2)+h,z,x,y,z});
            mVertices.push_back(Vertex{x+(h/2)+h,y+(h/2),z,x,y,z});
            mVertices.push_back(Vertex{x+(h/2)+h,y+(h/2)+h,z+(z-oldHeight),x,y,z});

            oldHeight = z;
            //finne duplicate edges og plusse de sammen / 2
        }
        width += 1;
    }
    height = height/width;

//    std::vector<unsigned int> indices;
//    for(unsigned int i = 0; i < height-1; i++)       // for each row a.k.a. each strip
//    {
//        for(unsigned int j = 0; j < width; j++)      // for each column
//        {
//            for(unsigned int k = 0; k < 2; k++)      // for each side of the strip
//            {
//                indices.push_back(j + width * (i + k));
//            }
//        }
//    }
}

void TriangleSurface::readFile(std::string filnavn) {
    std::ifstream fil;
    fil.open("../3Dprog22konte/"+filnavn);
    if (fil.is_open()) {
        int count;
        Vertex vertex;
        fil >> count;
        mVertices.reserve(count);
        float tempMinY;
        float tempMinX;
        for (int i=0; i<count; i++) {
             fil >> vertex;
             //vertex.divide();

             float x = vertex.getX();
             float y = vertex.getY();

             if (x > xMax){xMax = x;}
             if (y > yMax){yMax = y;}

             tempMinX = x;
             if (tempMinX < xMin){xMin = tempMinX;}

             tempMinY = y;
             if (tempMinY < yMin){yMin = tempMinY;}
             trianguler.push_back(vertex);
//             mVertices.push_back(vertex); //sender vertexene fra tekstfila inn i mVertices
        }
        fil.close();
    }
    mMatrix.setToIdentity();
    triangulate();
}

void TriangleSurface::init(GLint matrixUniform)
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
    glBindVertexArray(0);
}

void TriangleSurface::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

void TriangleSurface::construct(double (f)(double, double))
{
    float xmin=-20.0f, xmax=20.0f, ymin=-20.0f, ymax=20.0f, h=0.25f;
    for (auto x=xmin; x<xmax; x+=h)
        for (auto y=ymin; y<ymax; y+=h)
        {
            float z = f(x,y);
            mVertices.push_back(Vertex{x,y,z,x,y,z});
            z = f(x+h,y);
            mVertices.push_back(Vertex{x+h,y,z,x,y,z});
            z = f(x,y+h);
            mVertices.push_back(Vertex{x,y+h,z,x,y,z});
            mVertices.push_back(Vertex{x,y+h,z,x,y,z});
            z = f(x+h,y);
            mVertices.push_back(Vertex{x+h,y,z,x,y,z});
            z = f(x+h,y+h);
            mVertices.push_back(Vertex{x+h,y+h,z,x,y,z});
        }
solveIntegral(f);
    //writeFile("oblig1.txt"); //skriver til oblig1
//    float xmin=-5.0f, xmax=5.0f, ymin=-5.0f, ymax=5.0f, h=0.025f;
//     for (auto x=xmin; x<xmax; x+=h)
//         for (auto y=ymin; y<ymax; y+=h)
//         {
//             float z = sin(M_PI*x)*sin(M_PI*y);
//             mVertices.push_back(Vertex{x,y,z,x,y,z});
//             z = sin(M_PI*(x+h))*sin(M_PI*y);
//             mVertices.push_back(Vertex{x+h,y,z,0,1,0});
//             z = sin(M_PI*x)*sin(M_PI*(y+h));
//             mVertices.push_back(Vertex{x,y+h,z,0,0,0});
//             mVertices.push_back(Vertex{x,y+h,z,0,0,0});
//             z = sin(M_PI*(x+h))*sin(M_PI*y);
//             mVertices.push_back(Vertex{x+h,y,z,0,1,0});
//             z = sin(M_PI*(x+h))*sin(M_PI*(y+h));
//             mVertices.push_back(Vertex{x+h,y+h,z,0,0,0});
//         }
////    writeFile("test.txt"); 80mb/960k vertexer
}

double TriangleSurface::funksjon(double x,double y){  //funksjonen som integreres
    double f = pow(x,2) + y + 1/M_PI; //x^2+y+1/??
    return f;
}

double TriangleSurface::titty(double x, double y)
{
     return 0.5*cos(2*sqrt(pow(x, 2)+pow(y, 2))) - sqrt(x*x+y*y) * sqrt(x*x+y*y) * 0.015 + 0.5;
}

double TriangleSurface::ob2(double x, double y)
{
    return 0.3f * ( sin(sqrt((x*x)+(y*y)) ) + sqrt((x*x)+(y*y)) * sqrt((x*x)+(y*y)) * 0.0001f ) + 0.5f;
}

void TriangleSurface::solveIntegral(double(f)(double,double))
{
    //avgrenser omr??det til x: 0->1 og y: 0->1 og setter stepsize til 0.25 og starter med ?? si at integralet, x og y er alle 0
    double xMin = 0;
    double xMax = 1;
    double yMin = 0;
    double yMax = 1;
    double stepSize = 0.125;
    double integral = 0;
    double y = 0;
    double x = 0;

    x = xMin + 0.5*stepSize; //halverer stepsize for ?? finne midtpunkt i current square
    while (x < xMax)
    {
        y = yMin + 0.5*stepSize;
        while (y < yMax)
            {
                double volume = f(x,y)*pow(stepSize,2); //ganger med stepsize^2 for ?? f?? punktet ganget med hele arealet til current square
                integral = integral + volume;
                y = y + stepSize;
            }
        x = x + stepSize;
    }
    std::cout << integral;
    //analytisk = 5/6 + 1/pi
    //numerisk 0.5 stepSize = 1.13081
    //numerisk 0.25 stepSize = 1.14643
}
