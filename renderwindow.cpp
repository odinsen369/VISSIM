#include "renderwindow.h"
#include <QTimer>
#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>
#include <string>
#include "shader.h"
#include "mainwindow.h"
#include "logger.h"
#include "tetrahedron.h"
#include "trianglesurface.h"
#include "octahedronball.h"
#include "disc.h"
#include "npc.h"
#include "cube.h"
#include "meshgenerator.h"
#include "graphmaker.h"
#include <chrono>
#include "texture.h"
#include "light.h"
#include <QMouseEvent>
#include "heightmap.h"
#include "objmesh.h"

//brukte kode fra aleksander sin branch på github (som jeg var med å jobbe på for obliger)

RenderWindow::RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow)
    : mContext(nullptr), mInitialized(false), mMainWindow(mainWindow)

{
    mActiveCamera = &mCamera1;
    //This is sent to QWindow:
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(format);
    //Make the OpenGL context
    mContext = new QOpenGLContext(this);
    //Give the context the wanted OpenGL format (v4.1 Core)
    mContext->setFormat(requestedFormat());
    if (!mContext->create()) {
        delete mContext;
        mContext = nullptr;
        qDebug() << "Context could not be made - quitting this application";
    }

    mPmatrix = new QMatrix4x4{};
    mVmatrix = new QMatrix4x4{};
    //Make the gameloop timer:
    mRenderTimer = new QTimer(this);
    createObjects();
}

RenderWindow::~RenderWindow()
{
    //cleans up the GPU memory
    glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );
}

Vertex v1 = {0,0,10,0,1,0};
Vertex v2 = {10,0,20,1,2,0};
Vertex v3 = {10,0,0,1,0,0};
Vertex v4 = {20,0,10,0,1,0};

Vertex p1 = {-5,-25,5,0,1,0};
Vertex p2 = {25,25,5,1,0,0};
Vertex p3 = {-25,25,0,1,2,0};
Vertex p4 = {25,-25,5,0,1,0};

void RenderWindow::init()
{       
    //Have to do this, else program will crash (or you have to put in nullptr tests...)
    mLogger = Logger::getInstance();

    //This makes our render loop
    connect(mRenderTimer, SIGNAL(timeout()), this, SLOT(render()));

    //********************** General OpenGL stuff **********************
    if (!mContext->makeCurrent(this)) {
        mLogger->logText("makeCurrent() failed", LogType::REALERROR);
        return;}

    if (!mInitialized) {mInitialized = true;}

    initializeOpenGLFunctions();

    mLogger->logText("The active GPU and API:", LogType::HIGHLIGHT);
    std::string tempString;
    tempString += std::string("  Vendor: ") + std::string((char*)glGetString(GL_VENDOR)) + "\n" +
            std::string("  Renderer: ") + std::string((char*)glGetString(GL_RENDERER)) + "\n" +
            std::string("  Version: ") + std::string((char*)glGetString(GL_VERSION));
    mLogger->logText(tempString);

    startOpenGLDebugger();

    glEnable(GL_DEPTH_TEST);            //enables depth sorting - must then use GL_DEPTH_BUFFER_BIT in glClear
    //    glEnable(GL_CULL_FACE);       //draws only front side of models - usually what you want - test it out!
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);    //bakgrunnsfarge

    glEnable(GL_BLEND);// you enable blending function
       glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    CamOgShaderSetup();

    glBindVertexArray(0);       //unbinds any VertexArray - good practice
}

void RenderWindow::CamOgShaderSetup()
{
    mShaderProgram[0] = new Shader("../3Dprog22konte/plainshader.vert", "../3Dprog22konte/plainshader.frag");
    mShaderProgram[1] = new Shader("../3Dprog22konte/textureshader.vert", "../3Dprog22konte/textureshader.frag");
    mShaderProgram[2] = new Shader("../3Dprog22konte/phongshader.vert", "../3Dprog22konte/phongshader.frag");

    setUpShader(0);
    setUpTexture(1);
    setUpPhongShader(2);

    mTextureProgram[0] = new Texture();
    mTextureProgram[1] = new Texture("../3Dprog22konte/Assets/hund.bmp");
    mTextureProgram[2] = new Texture("../3Dprog22konte/Assets/aleks.bmp");
    mTextureProgram[3] = new Texture("../3Dprog22konte/Assets/gress.bmp");
    mTextureProgram[4] = new Texture("../3Dprog22konte/Assets/EksamenHeightmap.bmp");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTextureProgram[0]->id());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mTextureProgram[1]->id());

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, mTextureProgram[2]->id());

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, mTextureProgram[3]->id());

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, mTextureProgram[4]->id());

    for (auto it=mObjects.begin(); it!= mObjects.end(); it++)
        (*it)->init(mMmatrixUniform0);

    for (auto it=mDrawObjects.begin(); it!= mDrawObjects.end(); it++)
        (*it)->init(mMmatrixUniform0);

    // oppgave 5b
    mCamera1.init(mPmatrixUniform0,mVmatrixUniform0);
    mCamera1.perspective(80, 4.0/3.0, 0.1, 1000.0);

    // oppgave 5a
    mCamera2.init(mPmatrixUniform0,mVmatrixUniform0);
    mCamera2.perspective(80, 4.0/3.0, 0.1, 1000.0);

    if (player != nullptr)
        mActiveCamera->setTarget(player);
}

void RenderWindow::setUpShader(int id)
{
    mPmatrixUniform0 = glGetUniformLocation( mShaderProgram[id]->getProgram(), "pmatrix" );
    mVmatrixUniform0 = glGetUniformLocation( mShaderProgram[id]->getProgram(), "vmatrix" );
    mMmatrixUniform0 = glGetUniformLocation( mShaderProgram[id]->getProgram(), "matrix" );
}

void RenderWindow::setUpTexture(int id)
{
    mPmatrixUniform1 = glGetUniformLocation( mShaderProgram[id]->getProgram(), "pmatrix" );
    mVmatrixUniform1 = glGetUniformLocation( mShaderProgram[id]->getProgram(), "vmatrix" );
    mMmatrixUniform1 = glGetUniformLocation( mShaderProgram[id]->getProgram(), "matrix" );
    mTextureUniform = glGetUniformLocation(mShaderProgram[id]->getProgram(), "textureSampler");
}

void RenderWindow::setUpPhongShader(int id)
{
    mPmatrixUniform2 = glGetUniformLocation( mShaderProgram[id]->getProgram(), "pMatrix" );
    mVmatrixUniform2 = glGetUniformLocation( mShaderProgram[id]->getProgram(), "vMatrix" );
    mMmatrixUniform2 = glGetUniformLocation( mShaderProgram[id]->getProgram(), "mMatrix" );

    mLightColorUniform = glGetUniformLocation( mShaderProgram[id]->getProgram(), "lightColor" );
    mObjectColorUniform = glGetUniformLocation( mShaderProgram[id]->getProgram(), "objectColor" );
    mAmbientLightStrengthUniform = glGetUniformLocation( mShaderProgram[id]->getProgram(), "ambientStrengt" );
    mLightPositionUniform = glGetUniformLocation( mShaderProgram[id]->getProgram(), "lightPosition" );
    mSpecularStrengthUniform = glGetUniformLocation( mShaderProgram[id]->getProgram(), "specularStrength" );
    mSpecularExponentUniform = glGetUniformLocation( mShaderProgram[id]->getProgram(), "specularExponent" );
    mLightPowerUniform = glGetUniformLocation( mShaderProgram[id]->getProgram(), "lightPower" );
    mCameraPositionUniform = glGetUniformLocation( mShaderProgram[id]->getProgram(), "cameraPosition" );
    mTextureUniform2 = glGetUniformLocation(mShaderProgram[id]->getProgram(), "textureSampler");
}

void RenderWindow::createObjects()
{
    /////xyz//////
    XYZ = new InteractiveObject;
    XYZ->setDrawMethod(DrawMethod::Lines);
    XYZ->setVertices(MeshGenerator::xyz(100));
    mObjects.push_back(XYZ);

    /////mesh//////
    mesh = new ObjMesh("../3Dprog22konte/Assets/Monkey.obj");
    mObjects.push_back(mesh);

    /////light////// oppgave 3
    mLight = new Light;
    mLight->mMatrix.translate(10,10,10);
    mObjects.push_back(mLight);

    /////player///// oppgave 4
     player = new InteractiveObject;
     player->setDrawMethod(DrawMethod::Triangles);
     player->setVertices(MeshGenerator::CubeMaker()); //temp player
     player->hasGravity = true;
     mObjects.push_back(player);

     /////bakke////// oppgave 2
     bakke = new HeightMap("../3Dprog22konte/Assets/EksamenHeightmap.bmp", 3, 0.1); //endre texture number i draw
     bakke->mMatrix.translate(-15,-25,0); //translate alt med -15,-25,0
     bakke->mMatrix.scale(1,1,1);
     mObjects.push_back(bakke);

     player->hmfloor = bakke;

     /////dot//////
//     dot = new InteractiveObject;
//     dot->setVertices((MeshGenerator::Dot()));
//     mObjects.push_back(dot);

//    bezier = new InteractiveObject;
//    bezier->setDrawMethod(DrawMethod::LineStrip);
//    bezier->setVertices(MeshGenerator::makeBezier(v1,v2,v3,v4));
//    mObjects.push_back(bezier);

    /////npc///// oppgave 10
    npc = new NPC();
    npc->setVertices(MeshGenerator::createcube());
    npc->setPosition(0,0,1);
    npc->trofe = &redTrophies;
    mObjects.push_back(npc);


//    fence = new InteractiveObject;
//    fence->setVertices(MeshGenerator::Fence());
//    fence->setPosition(2,3,1);
//    mDrawObjects.push_back(fence);
//    fence = new InteractiveObject;
//    fence->setVertices(MeshGenerator::Fence());
//    fence->setPosition(10,5,1);
//    fence->rotate(90.f);
//    mDrawObjects.push_back(fence);


    //oppgave 9
    for (int i = 0; i < 20; i++) {
//        Sleep(20); fiks runtime delay
        trophy = new InteractiveObject;
        trophy->setVertices(MeshGenerator::createColoredCube(i % 2 == 0 ? "red" : "blue"));
        trophy->move(rand() % 30 - 15, rand() % 50 - 25, 5.f);
        if (i % 2 != 0)
        {
            blueTrophies.push_back(trophy);
        }
        if (i % 2 == 0)
        {
            redTrophies.push_back(trophy);
        }
        i % 2 == 0 ? redTrophy += 1 : blueTrophy += 1;
        mDrawObjects.push_back(trophy);
    }

    //spiller har vektor <trophy*> og sjekker etter overlap i tick
    //loop gjennom lista med trophy og sjekk avstand if overlap, size = 0 og bool pickedUp
    cameramesh = new InteractiveObject;
    cameramesh->setVertices(MeshGenerator::createcube());
    cameramesh->setPosition(mCamera1.getForward().x(),mCamera1.getForward().x(), mCamera1.getForward().x());
    mDrawObjects.push_back(cameramesh);
}

void RenderWindow::render()
{
    mTimeStart.restart();
    mContext->makeCurrent(this);
    initializeOpenGLFunctions();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //~~ Tick
        auto time1 = std::chrono::system_clock::now();
        static auto time2 = std::chrono::system_clock::now();
        std::chrono::duration<double, std::milli> duration = time1 - time2;
        time2 = std::chrono::system_clock::now();
        Tick(duration.count() / 1000);

        Drawcall();

    calculateFramerate();

    checkForGLerrors();

    mContext->swapBuffers(this);
}

void RenderWindow::Drawcall()
{
    ////tegner lys
    glUseProgram(mShaderProgram[2]->getProgram() );
    glUniformMatrix4fv(mMmatrixUniform2, 1, GL_FALSE, mLight->mMatrix.constData());
    mLight->draw();

    ////tegner xyz
    glUseProgram(mShaderProgram[0]->getProgram() );
    mActiveCamera->update(mPmatrixUniform0, mVmatrixUniform0);
    glUniformMatrix4fv(mMmatrixUniform1, 1, GL_FALSE, XYZ->mMatrix.constData());
    XYZ->draw();

    ////player oppgave 4
    glUseProgram(mShaderProgram[2]->getProgram());
    glUniformMatrix4fv( mVmatrixUniform2, 1, GL_FALSE, mActiveCamera->mVmatrix.constData());
    glUniformMatrix4fv( mPmatrixUniform2, 1, GL_FALSE, mActiveCamera->mPmatrix.constData());
    glUniformMatrix4fv( mMmatrixUniform2, 1, GL_FALSE, player->mMatrix.constData());

    glUniform3f(mLightPositionUniform, mLight->mMatrix.column(3).x(), mLight->mMatrix.column(3).y(),
              mLight->mMatrix.column(3).z());
    glUniform3f(mCameraPositionUniform, mActiveCamera->GetPosition().x(), mActiveCamera->GetPosition().y(), mActiveCamera->GetPosition().z());
    glUniform3f(mLightColorUniform, mLight->mLightColor.x(), mLight->mLightColor.y(), mLight->mLightColor.z());
    glUniform1f(mSpecularStrengthUniform, mLight->mSpecularStrenght);

    glUniform1i(mTextureUniform2, 2);
    mesh->draw();

    ////oppgave 2 (bakke)
    glUseProgram(mShaderProgram[2]->getProgram());
    glUniformMatrix4fv( mVmatrixUniform2, 1, GL_FALSE, mActiveCamera->mVmatrix.constData());
    glUniformMatrix4fv( mPmatrixUniform2, 1, GL_FALSE, mActiveCamera->mPmatrix.constData());
    glUniformMatrix4fv( mMmatrixUniform2, 1, GL_FALSE, bakke->mMatrix.constData());

    glUniform3f(mLightPositionUniform, mLight->mMatrix.column(3).x(), mLight->mMatrix.column(3).y(),
    mLight->mMatrix.column(3).z());
    glUniform3f(mCameraPositionUniform, mActiveCamera->GetPosition().x(), mActiveCamera->GetPosition().y(), mActiveCamera->GetPosition().z());
    glUniform3f(mLightColorUniform, mLight->mLightColor.x(), mLight->mLightColor.y(), mLight->mLightColor.z());
    glUniform1f(mSpecularStrengthUniform, mLight->mSpecularStrenght);

    glUniform1i(mTextureUniform2, 3); // 3 her bruker texture 3
    bakke->draw();

    ////dot
//    glUseProgram(mShaderProgram[0]->getProgram() );
//    mActiveCamera->update(mPmatrixUniform0, mVmatrixUniform0);
//    glUniformMatrix4fv(mMmatrixUniform1, 1, GL_FALSE, dot->mMatrix.constData());
//    dot->draw();

    ////bezier
//    glUseProgram(mShaderProgram[0]->getProgram() );
//    mActiveCamera->update(mPmatrixUniform0, mVmatrixUniform0);
//    glUniformMatrix4fv(mMmatrixUniform1, 1, GL_FALSE, bezier->mMatrix.constData());
//    bezier->draw();

    ////npc
    glUseProgram(mShaderProgram[0]->getProgram() );
    mActiveCamera->update(mPmatrixUniform0, mVmatrixUniform0);
    glUniformMatrix4fv(mMmatrixUniform1, 1, GL_FALSE, npc->mMatrix.constData());
    npc->draw();

    ////tegner opp objektene i mDrawObjects
    for (auto it=mDrawObjects.begin(); it!= mDrawObjects.end(); it++)
    {
        glUniformMatrix4fv(mMmatrixUniform0, 1, GL_FALSE, (*it)->mMatrix.constData());
        (*it)->draw();
    }
}

void RenderWindow::exposeEvent(QExposeEvent *)
{
    if (!mInitialized)
        init();

    const qreal retinaScale = devicePixelRatio();

    glViewport(0, 0, static_cast<GLint>(width() * retinaScale), static_cast<GLint>(height() * retinaScale));

    if (isExposed())
    {
        mRenderTimer->start(16);
        mTimeStart.start();
    }
}

void RenderWindow::calculateFramerate()
{
    long nsecElapsed = mTimeStart.nsecsElapsed();
    static int frameCount{0};                       //counting actual frames for a quick "timer" for the statusbar

    if (mMainWindow)            //if no mainWindow, something is really wrong...
    {
        ++frameCount;
        if (frameCount > 30)    //once pr 30 frames = update the message == twice pr second (on a 60Hz monitor)
        {
            //showing some statistics in status bar
            mMainWindow->statusBar()->showMessage(" Time pr FrameDraw: " +
                                                  QString::number(nsecElapsed/1000000.f, 'g', 4) + " ms  |  " +
                                                  "FPS (approximated): " + QString::number(1E9 / nsecElapsed, 'g', 7));
            frameCount = 0;     //reset to show a new message in 30 frames
        }
    }
}

void RenderWindow::checkForGLerrors()
{
    if(mOpenGLDebugLogger)  //if our machine got this class to work
    {
        const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
        {
            if (!(message.type() == message.OtherType)) // get rid of uninteresting "object ...
                                                        // will use VIDEO memory as the source for
                                                        // buffer object operations"
                // valid error message:
                mLogger->logText(message.message().toStdString(), LogType::REALERROR);
        }
    }
    else
    {
        GLenum err = GL_NO_ERROR;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            mLogger->logText("glGetError returns " + std::to_string(err), LogType::REALERROR);
            switch (err) {
            case 1280:
                mLogger->logText("GL_INVALID_ENUM - Given when an enumeration parameter is not a "
                                "legal enumeration for that function.");
                break;
            case 1281:
                mLogger->logText("GL_INVALID_VALUE - Given when a value parameter is not a legal "
                                "value for that function.");
                break;
            case 1282:
                mLogger->logText("GL_INVALID_OPERATION - Given when the set of state for a command "
                                "is not legal for the parameters given to that command. "
                                "It is also given for commands where combinations of parameters "
                                "define what the legal parameters are.");
                break;
            }
        }
    }
}

void RenderWindow::startOpenGLDebugger()
{
    QOpenGLContext * temp = this->context();
    if (temp)
    {
        QSurfaceFormat format = temp->format();
        if (! format.testOption(QSurfaceFormat::DebugContext))
            mLogger->logText("This system can not use QOpenGLDebugLogger, so we revert to glGetError()",
                             LogType::HIGHLIGHT);

        if(temp->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
        {
            mLogger->logText("This system can log extended OpenGL errors", LogType::HIGHLIGHT);
            mOpenGLDebugLogger = new QOpenGLDebugLogger(this);
            if (mOpenGLDebugLogger->initialize()) // initializes in the current context
                mLogger->logText("Started Qt OpenGL debug logger");
        }
    }
}

void RenderWindow::keyPressEvent(QKeyEvent *event)
{
    static bool wireframe = false;
    mCurrentInputs[event->key()] = true;

    if (event->key() == Qt::Key_Escape)
    {
        mMainWindow->close();       //Shuts down the whole program
    }

    if(event->key() == Qt::Key_3)
    {
        if (wireframe == false)
        {
            for (auto it=mObjects.begin(); it!= mObjects.end(); it++)
                (*it)->setDrawMethod(DrawMethod::LineLoop);
            for (auto it=mDrawObjects.begin(); it!= mDrawObjects.end(); it++)
                (*it)->setDrawMethod(DrawMethod::LineLoop);
            wireframe = true;
        }
        else
        {
            for (auto it=mObjects.begin(); it!= mObjects.end(); it++)
                (*it)->setDrawMethod(DrawMethod::Triangles);
            for (auto it=mDrawObjects.begin(); it!= mDrawObjects.end(); it++)
                (*it)->setDrawMethod(DrawMethod::Triangles);
            wireframe = false;
        }
    }
}

void RenderWindow::keyReleaseEvent(QKeyEvent *event)
{
    mCurrentInputs[event->key()] = false;
}

void RenderWindow::wheelEvent(QWheelEvent *event)
{
    mActiveCamera->Zoom(event->angleDelta().y());
}

void RenderWindow::Tick(float deltaTime)
{
    mActiveCamera->Tick(deltaTime);
    player->Tick(deltaTime);
    Movement(deltaTime);
    npc->Tick(deltaTime);
    for (int i = 0; i < blueTrophies.size(); i++)
    {
        if (blueTrophies.at(i) != nullptr && abs(player->getPosition().x() - blueTrophies.at(i)->getPosition().x()) < 1 && abs(player->getPosition().y() - blueTrophies.at(i)->getPosition().y()) < 1)
        {
            blueTrophy -= 1;
            qDebug() << "size :" << blueTrophy;
            blueTrophies.at(i)->setScale(0);
            blueTrophies.at(i) = nullptr;
        }
    }

    for (int i = 0; i < redTrophies.size(); i++)
    {
        if (redTrophies.at(i) != nullptr && abs(npc->getPosition().x() - redTrophies.at(i)->getPosition().x()) < 1 && abs(npc->getPosition().y() - redTrophies.at(i)->getPosition().y()) < 1)
        {
            redTrophy -= 1;
            qDebug() << "size :" << redTrophy;
            redTrophies.at(i)->setScale(0);
            redTrophies.at(i) = nullptr;
        }
    }

    if (blueTrophy <= 0 && redTrophy > 0 && victory == false)
    {
        qDebug() << "victory royale";
        victory = true;
    }
    if (blueTrophy > 0 && redTrophy <= 0 && victory == false)
    {
        qDebug() << "npc won";
        victory = true;
    }
}

void RenderWindow::Movement(float deltaTime)
{
    QVector3D camPos = mCamera1.GetPosition();
    camPos.setZ(camPos.z() - 2);
    cameramesh->setPosition(camPos);
    QVector3D newpos = player->getPosition();

    if (mToggle == true)
    {
        mActiveCamera = &mCamera1;
        if (player != nullptr)
        {
            mActiveCamera->setTarget(player);
        }
        XYZ->setScale(0);
    }
    if (mToggle == false)
    {
        mActiveCamera = &mCamera2;
        if (dot != nullptr)
        {
            mActiveCamera->setTarget(dot);
        }
        XYZ->setScale(100);
    }


    if (reset == true)
    {
        dot->setPosition(0 + newpos.x(),0 + newpos.y(),2 + newpos.z());
        player->setPosition(0 + newpos.x(),0 + newpos.y(),2 + newpos.z());
        reset = false;
    }


    ////Light movement
    if (mLight)
    {
        if (time <= 300 && right == true)
        {
        mLight->mMatrix.translate(0.1f,0,0);
        time += 1;
        }
        else
        {
        right = false;
        mLight->mMatrix.translate(-0.1f,0,0);
        time -= 1.f/2.f;
        if (time <= 0)
        {right = true;}
        }
    }

    ////Player movement
    if(player && mActiveCamera == &mCamera1)
    {
        if(mCurrentInputs[Qt::Key_W]) //frem
        {
            auto fwd = mActiveCamera->getForward();
            fwd.setZ(0);
            fwd.normalize();
            fwd = fwd / 5;
            player->push(fwd,0.03);
        }
        if(mCurrentInputs[Qt::Key_S]) //tilbake
        {
            auto fwd = mActiveCamera->getForward();
            fwd.setZ(0);
            fwd.normalize();
            fwd = fwd / 5;
            player->push(-fwd,0.1);
        }
        if(mCurrentInputs[Qt::Key_A] || mCurrentInputs[Qt::Key_D])
        {
            if(mCurrentInputs[Qt::Key_A]) //roter høyre
            {
                player->rotate(1.f);
                mActiveCamera->followMouse(1,0);
            }
            if(mCurrentInputs[Qt::Key_D]) //roter venstre
            {
                player->rotate(-1.f);
                mActiveCamera->followMouse(-1,0);
            }
        }
        if(mCurrentInputs[Qt::Key_Space])
        {
            player->push({0,0,1},1);
        }
    }

    if(dot && mActiveCamera == &mCamera2){
        if(mCurrentInputs[Qt::Key_W])
        {
            dot->move(0.0f,0.2f,0.0f);
        }
        if(mCurrentInputs[Qt::Key_A])
        {
            dot->move(-0.2f,0.0f,0.0f);
        }
        if(mCurrentInputs[Qt::Key_S])
        {
            dot->move(0.0f,-0.2f,0.0f);
        }
        if(mCurrentInputs[Qt::Key_D])
        {
            dot->move(0.2f,0.0f,0.0f);
        }
        if(mCurrentInputs[Qt::Key_E])
        {
            dot->move(0.0f,0.0f,0.2f);
        }
        if(mCurrentInputs[Qt::Key_Q])
        {
            dot->move(0.0f,0.0f,-0.2f);
        }
    }
}

void RenderWindow::mousePressEvent(QMouseEvent *event)
{
    mCurrentInputs[event->button()] = true;
    if (mCurrentInputs[event->button() == Qt::LeftButton])
    {
        if (dot)
        {
            mCamera2.mPmatrix.rotate(-15,{0,1,0.3});
        }
    }
    if (mCurrentInputs[event->button() == Qt::RightButton])
    {
        if (dot)
        {
            mCamera2.mPmatrix.rotate(15,{0,1,0.3});
        }
    }
}
