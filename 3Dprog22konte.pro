QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = 3D-programmering

mac {
    LIBS += -framework OpenAL
}

win32 {
    INCLUDEPATH += $(OPENAL_HOME)\\include\\AL
    LIBS *= $(OPENAL_HOME)\\libs\\Win64\\libOpenAL32.dll.a
}

SOURCES += main.cpp \
    MeshGenerator.cpp \
    camera.cpp \
    cube.cpp \
    graphmaker.cpp \
    heightmap.cpp \
    interactiveobject.cpp \
    light.cpp \
    logger.cpp \
    mainwindow.cpp \
    objmesh.cpp \
    octahedronball.cpp \
    renderwindow.cpp \
    rollingball.cpp \
    shader.cpp \
    soundmanager.cpp \
    soundsource.cpp \
    terrain.cpp \
    texture.cpp \
    trianglesurface.cpp \
    vertex.cpp \
    visualobject.cpp \
    wavfilehandler.cpp

HEADERS += \
    Triangle.h \
    Vector3d.h \
    camera.h \
    cube.h \
    graphmaker.h \
    heightmap.h \
    interactiveobject.h \
    light.h \
    logger.h \
    mainwindow.h \
    meshgenerator.h \
    objmesh.h \
    octahedronball.h \
    renderwindow.h \
    rollingball.h \
    shader.h \
    soundmanager.h \
    soundsource.h \
    terrain.h \
    texture.h \
    trianglesurface.h \
    vertex.h \
    visualobject.h \
    wavfilehandler.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    phongshader.frag \
    phongshader.vert \
    plainshader.frag \
    plainshader.vert \
    textureshader.frag \
    textureshader.vert
