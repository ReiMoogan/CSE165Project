#include <QApplication>
#include <QSurfaceFormat>

#include "Window.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(resources);

    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    Window window;
    window.show();
    return QApplication::exec();
}
