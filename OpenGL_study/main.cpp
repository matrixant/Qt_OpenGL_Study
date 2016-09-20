//#include "myglwidget.h"
#include "openglwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Set OpenGL version information
    //This format must be set before show() is called.
    QSurfaceFormat mFormat;
    //Set render type
    mFormat.setRenderableType(QSurfaceFormat::OpenGL);
    mFormat.setProfile(QSurfaceFormat::CoreProfile);
    mFormat.setVersion(3, 3);

    OpenGLWindow mWindow;
    mWindow.setFormat(mFormat);
    mWindow.show();

    return a.exec();
}
