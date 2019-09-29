#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include "frameprincipale.h"

int main(int argc, char *argv[])
{
    QApplication MyApp(argc, argv);

    QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    translator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    MyApp.installTranslator(&translator);

    FramePrincipale frameprincipale;
    frameprincipale.show();

    return MyApp.exec();
}
