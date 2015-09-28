#include <QtWidgets/qapplication.h>

#include "main_interface.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    MainInterface iface;
    iface.showMaximized();
    return app.exec();
}
