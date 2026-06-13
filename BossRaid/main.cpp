#include "mainwindow.h"
#include <QApplication>
#include <QTimer>

extern int uruchomTestyLogiki();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // ODPALENIE TESTÓW JEDNOSTKOWYCH
    qDebug() << "=== URUCHAMIANIE TESTÓW JEDNOSTKOWYCH ===";
    uruchomTestyLogiki();
    qDebug() << "=========================================\n";

    // URUCHOMIENIE WŁAŚCIWEJ GRY
    MainWindow w;
    w.show();
    return a.exec();
}