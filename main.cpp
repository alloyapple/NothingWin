#include <QApplication>
#include <QMessageBox>
#include <QDebug>

#include "src/mainwindow.h"
#include "src/settings.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);
    app.setApplicationName("NothingHere");
    app.setOrganizationName("NothingHere");
    app.setApplicationVersion("1.0.0");

    Settings settings;
    MainWindow w;
    w.init();
    w.hide();

    return app.exec();
}
