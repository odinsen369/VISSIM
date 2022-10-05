#include "terrain.h"

terrain::terrain()
{

}

terrain::~terrain()
{

}

terrain::terrain(std::string filnavn)
{
   readFile(filnavn);
   mMatrix.setToIdentity();
}

void terrain::readFile(std::string filnavn) {
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
        }
        fil.close();
    }
    triangulate();
}

void terrain::triangulate()
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
            height += 1;
            z = z/amount;

            mVertices.push_back(Vertex{x+(h/2),y+(h/2),z,0,0.4,0});
            mVertices.push_back(Vertex{x+(h/2)+h,y+(h/2),z,0,0.5,0});
            mVertices.push_back(Vertex{x+(h/2),y+(h/2)+h,z,0,0.6,0});
            mVertices.push_back(Vertex{x+(h/2),y+(h/2)+h,z,0,0.5,0});
            mVertices.push_back(Vertex{x+(h/2)+h,y+(h/2),z,0,0.4,0});
            mVertices.push_back(Vertex{x+(h/2)+h,y+(h/2)+h,z,0,0.6,0});
        }
        width += 1;
    }
    height = height/width;
    qDebug() << width << " " << height;

    buildMesh();
}

void terrain::buildMesh()
{
   for (int i = 0; i < width; i++)
   {
       for (int j = 0; j < height; j++)
       {
           mIndices.push_back(i);
           mIndices.push_back(i+1);
           mIndices.push_back(i+width+1);
           mIndices.push_back(j+1);
           mIndices.push_back(j+width+1);
           mIndices.push_back(j+width+2);
       }
    }
}

void terrain::init(GLint matrixUniform)
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)( 6 * sizeof( GLfloat ) ));
    glEnableVertexAttribArray(2);

    //Second buffer - holds the indices (Element Array Buffer - EAB):
    glGenBuffers(1, &mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void terrain::draw()
{
    glBindVertexArray( mVAO );
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
    glBindVertexArray(1);
}
