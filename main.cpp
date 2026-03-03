#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QIcon>

#include "src/mainwindow.h"
#include "src/settings.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);
    app.setApplicationName("NothingWin");
    app.setOrganizationName("NothingWin");
    app.setApplicationVersion("1.0.0");
    app.setWindowIcon(QIcon("logo.png"));

    Settings settings;
    MainWindow w;
    w.init();
    w.hide();

    return app.exec();
}
