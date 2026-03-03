#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QIcon>
#include <QTranslator>
#include <QLocale>
#include <QDir>

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

    // Load translator based on system locale
    QTranslator *translator = new QTranslator();
    QString locale = QLocale::system().name().left(2);
    QString translationsDir = QCoreApplication::applicationDirPath() + "/translations";
    
    if (translator->load("nothingwin_" + locale, translationsDir)) {
        app.installTranslator(translator);
        qDebug() << "Loaded translation for:" << locale;
    } else {
        delete translator;
        qDebug() << "No translation found for:" << locale << ", using default (English)";
    }

    Settings settings;
    MainWindow w;
    w.init();
    w.hide();

    return app.exec();
}
