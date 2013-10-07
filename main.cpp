#include "mycash.h"
#include <QtGui>
#include <QApplication>
#include "global.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator, tr;

    tr.load("mycash_" + QLocale::system().name(), ".");
    a.installTranslator(&tr);

    translator.load("qt_ru", QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&translator);

    default_locale = new QLocale();
    english = new QLocale(QLocale::Ukrainian);

    qDebug() << default_locale->currencySymbol(default_locale->CurrencyDisplayName);


    MyCash w;
    w.show();
    
    return a.exec();
}
