#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "kartawidget.h"
#include <QMainWindow>
#include "Gracz.h"
#include "Boss.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; } // To musi zostać w takiej krótkiej formie!
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    // TUTAJ dajemy naszą nową funkcję:
    void dodajLosowaKarte();
    Gracz *aktualnyGracz;
    Boss *bossRaidu;
    void aktualizujInterfejs();
    void dodajKomunikatGracza(const QString &tekst);
    void dodajKomunikatBossa(const QString &tekst);
    void sprawdzKoniecGry(int ostatnieObrazenia = 0); // Domyślnie 0, żeby stare wywołania się nie wywaliły
    std::vector<KartaWidget*> widgetyWRece;


private slots:
    void onKartaKliknieta(KartaWidget* kliknietaKarta);
    void zagrajWybranaKarte(); // Od mieczy (Atak)
    void zakonczTure();        // Od końca tury
    void przerolujKarty();
};

#endif // MAINWINDOW_H