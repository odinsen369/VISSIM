QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = 3D-programmering

SOURCES += main.cpp \
    MeshGenerator.cpp \
    camera.cpp \
    cube.cpp \
    disc.cpp \
    graphmaker.cpp \
    heightmap.cpp \
    interactiveobject.cpp \
    light.cpp \
    logger.cpp \
    mainwindow.cpp \
    npc.cpp \
    objmesh.cpp \
    octahedronball.cpp \
    renderwindow.cpp \
    shader.cpp \
    tetrahedron.cpp \
    texture.cpp \
    trianglesurface.cpp \
    vertex.cpp \
    visualobject.cpp

HEADERS += \
    camera.h \
    cube.h \
    disc.h \
    graphmaker.h \
    heightmap.h \
    interactiveobject.h \
    light.h \
    logger.h \
    mainwindow.h \
    meshgenerator.h \
    npc.h \
    objmesh.h \
    octahedronball.h \
    renderwindow.h \
    shader.h \
    tetrahedron.h \
    texture.h \
    trianglesurface.h \
    vertex.h \
    visualobject.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    phongshader.frag \
    phongshader.vert \
    plainshader.frag \
    plainshader.vert \
    textureshader.frag \
    textureshader.vert
