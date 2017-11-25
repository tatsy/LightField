#include <qapplication.h>
#include <qsurfaceformat.h>

#include "maingui.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    format.setMajorVersion(4);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setOption(QSurfaceFormat::DeprecatedFunctions, false);
    QSurfaceFormat::setDefaultFormat(format);

    MainGui mainGui;
    mainGui.show();

    return app.exec();
}
