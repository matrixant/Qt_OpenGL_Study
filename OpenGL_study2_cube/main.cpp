#include "openglwidget.h"
#include <QApplication>
#include <QPushButton>
#include <QHBoxLayout>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    QSurfaceFormat mSurface;
//    mSurface.setRenderableType(QSurfaceFormat::OpenGL);
//    mSurface.setProfile(QSurfaceFormat::CoreProfile);
//    mSurface.setVersion(3, 3);

    OpenGLWidget w;
//    w.setFormat(mSurface);
    w.show();

//    OpenGLWidget *mGLWidget = new OpenGLWidget();

//    QWidget mWidget;
//    QPushButton *mButton = new QPushButton("Ok");
//    QVBoxLayout *mLayout = new QVBoxLayout();

//    mLayout->setGeometry(QRect(0, 0, 320, 240));
//    mLayout->addWidget(mGLWidget);
//    mLayout->addWidget(mButton);

//    mWidget.setLayout(mLayout);
//    mWidget.show();


    return a.exec();
}
