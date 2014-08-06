#include "mycash.h"
#include <QtGui>
#include <QApplication>
#include "global.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSplashScreen splash(QPixmap(":icons/mycash.jpe"));
    QTranslator translator, tr;

    splash.show();

    tr.load("mycash_" + QLocale::system().name(), ".");
    a.installTranslator(&tr);

    translator.load("qt_ru", QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&translator);

    default_locale = new QLocale();

    MyCash w;

    splash.finish(&w);

    w.show();
    
    return a.exec();
}
