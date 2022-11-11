#include "mainwindow.h"
#include <QApplication>
#include "soundmanager.h"

int main(int argc, char *argv[])
{
    //Forces the usage of desktop OpenGL - Qt uses OpenGL ES as default
    //Attribute must be set before Q(Gui)Application is constructed:
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);

    //Makes a Qt application
    QApplication a(argc, argv);

    //Makes the Qt MainWindow and shows it.
    MainWindow w;
    w.show();

    w.resize(900, 600);

    SoundManager::getInstance()->cleanUp();

    return a.exec();
}
