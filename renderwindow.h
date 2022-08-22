#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QElapsedTimer>
#include <vector>
#include "vertex.h"
#include "visualobject.h"
#include "interactiveobject.h"
#include "camera.h"
#include "disc.h"
#include "npc.h"
#include "graphmaker.h"
#include "texture.h"
#include "light.h"
#include "heightmap.h"
#include "octahedronball.h"
#include "objmesh.h"

class QOpenGLContext;
class Shader;
class MainWindow;

/// This inherits from QWindow to get access to the Qt functionality and
// OpenGL surface.
// We also inherit from QOpenGLFunctions, to get access to the OpenGL functions
// This is the same as using "glad" and "glw" from general OpenGL tutorials
class RenderWindow : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow);
    ~RenderWindow() override;
    void Tick(float deltaTime);

    QOpenGLContext *context() { return mContext; }

    void exposeEvent(QExposeEvent *) override;  //gets called when app is shown and resized

    bool mToggle{true};
    bool reset{false};

private slots:
    void render();          //the actual render - function

private:
    void init();            //initialize things we need before rendering
    void Drawcall();
    void CamOgShaderSetup();

    QOpenGLContext *mContext{nullptr};  //Our OpenGL context
    bool mInitialized{false};

    Shader *mShaderProgram[4]{nullptr};    //holds pointer the GLSL shader program
    ShaderInfo mShaderInfo[4];
    Texture *mTextureProgram[6]{nullptr};

    //GLint  mMatrixUniform;              //OpenGL reference to the Uniform in the shader program
    void setUpShader(int id);
    GLint mPmatrixUniform0;
    GLint mVmatrixUniform0;
    GLint mMmatrixUniform0;

    void setUpTexture(int id);
    GLint mPmatrixUniform1;
    GLint mVmatrixUniform1;
    GLint mMmatrixUniform1;
    GLint mTextureUniform;

    void setUpPhongShader(int id);
    GLint mPmatrixUniform2;
    GLint mVmatrixUniform2;
    GLint mMmatrixUniform2;

    GLint mLightColorUniform{-1};
    GLint mObjectColorUniform{-1};
    GLint mAmbientLightStrengthUniform{-1};
    GLint mLightPositionUniform{-1};
    GLint mCameraPositionUniform{-1};
    GLint mSpecularStrengthUniform{-1};
    GLint mSpecularExponentUniform{-1};
    GLint mLightPowerUniform{-1};
    GLint mTextureUniform2{-1};

    GLuint mVAO;                        //OpenGL reference to our VAO
    GLuint mVBO;                        //OpenGL reference to our VBO

    QMatrix4x4 *mMVPmatrix{nullptr};         //The matrix with the transform for the object we draw
    QMatrix4x4 *mPmatrix{nullptr};
    QMatrix4x4 *mVmatrix{nullptr};

    QTimer *mRenderTimer{nullptr};           //timer that drives the gameloop
    QElapsedTimer mTimeStart;               //time variable that reads the calculated FPS

    MainWindow *mMainWindow{nullptr};        //points back to MainWindow to be able to put info in StatusBar

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};  //helper class to get some clean debug info from OpenGL
    class Logger *mLogger{nullptr};         //logger - Output Log in the application

    ///Helper function that uses QOpenGLDebugLogger or plain glGetError()
    void checkForGLerrors();

    void calculateFramerate();          //as name says

    ///Starts QOpenGLDebugLogger if possible
    void startOpenGLDebugger();

    void createObjects();
    void Movement(float deltaTime);
    static inline auto currentGraph = GraphMaker::my2DEquation;


protected:
    //The QWindow that we inherit from have these functions to capture
    // - mouse and keyboard.
    // Uncomment to use (you also have to make the definitions of
    // these functions in the cpp-file to use them of course!)
    //
    void mousePressEvent(QMouseEvent *event) override;
//    void mouseReleaseEvent(QMouseEvent *event) override;
//    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;              //the only one we use now
    void keyReleaseEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    std::unordered_map<int, bool> mCurrentInputs;

public:
    std::vector<VisualObject*> mObjects;
    std::vector<VisualObject*> mDrawObjects;
    InteractiveObject* player{nullptr};
    InteractiveObject* graph{nullptr};
    InteractiveObject* plane{nullptr};
    InteractiveObject* dot{nullptr};
    InteractiveObject* bezier{nullptr};
    InteractiveObject* trophy{nullptr};
    InteractiveObject* fence{nullptr};
    InteractiveObject* XYZ{nullptr};
    InteractiveObject* cameramesh{nullptr};

    Light* mLight{nullptr};
    InteractiveObject* ball{nullptr};

    Camera mCamera1;
    Camera mCamera2;
    Camera* mActiveCamera{nullptr};

    Disc* disc{nullptr};
    NPC* npc{nullptr};
    NPC* bombNPC{nullptr};
    HeightMap* bakke{nullptr};
    ObjMesh* mesh{nullptr};

    int redTrophy {0}, blueTrophy {0};
    std::vector<InteractiveObject*> blueTrophies;
    std::vector<InteractiveObject*> redTrophies;
    bool victory = false;
    float time {0};
    bool right = true;
};

#endif // RENDERWINDOW_H
