#include "npc.h"
#include "cube.h"
#include "graphmaker.h"


NPC::NPC()
{
    mVertices.push_back(Vertex{0,0,0,1,1,0});
    mVertices.push_back(Vertex{1,0,0,1,1,0});
    mVertices.push_back(Vertex{0,1,1,1,1,0});
}

NPC::NPC(float (f)(float))
{
    float xmin=-10.0f, xmax=10.0f, h=0.1f;
    for (auto x=xmin; x<xmax; x+=h)
    {
        punkter.push_back(Vertex{x,0,f(x),x,0,f(x)});
    }

    mVertices.push_back(Vertex{0,0,0,1,1,0,0,0});
    mVertices.push_back(Vertex{1,0,0,1,1,0,1,0});
    mVertices.push_back(Vertex{0,1,1,1,1,0,0.5,1});
    mMatrix.setToIdentity();
}

NPC::NPC(Vertex v1, Vertex v2, Vertex v3, Vertex v4)
{
    Vertex temp{0,0,0,0,0};
    for (float x = 0; x<1; x+=0.01f)
    {
        temp = GraphMaker::CalculateBezier(v1,v2,v3,v4,x); //sender inn punkter
        punkter.push_back(temp);
    }
}

NPC::NPC(InteractiveObject p1)
{

}

NPC::~NPC() {}

void NPC::init(GLint matrixUniform)
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

void NPC::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

void NPC::move()
{
//        const static unsigned long long int nanosecondsforassassintooneshotadc = 2;
        static int i = 1;
        static int direction = 1;
        if (i == punkter.size()-1)
        {
            direction = -1;
        }
        else if (i == 0)
        {
            direction = 1;
        }
        setPosition(punkter.at(i).m_xyz[0],0,punkter.at(i).m_xyz[2]);
        i += direction;
}

//oppgave 10
void NPC::Tick(float deltaTime) {
  InteractiveObject* trophyFound = nullptr;
  for (auto p : *trofe) {
    if (p != nullptr) {
      trophyFound = p;
      break;
    }
  }
  if (trophyFound != nullptr) {
    QVector3D dir = trophyFound->getPosition() - getPosition();
    dir.normalize();
    InteractiveObject::move(dir * deltaTime * 3);
  }
}
