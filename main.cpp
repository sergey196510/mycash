#include "mycash.h"
#include <QtGui>
#include <QApplication>
#include "global.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;

    translator.load("qt_ru", QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&translator);

//    locale = new QLocale(QLocale::Russian, QLocale::RussianFederation);
    default_locale = new QLocale();
    english = new QLocale(QLocale::C);

    MyCash w;
    w.show();
    
    return a.exec();
}
