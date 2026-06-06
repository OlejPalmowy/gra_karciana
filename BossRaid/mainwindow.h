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

private slots:
    void onKartaKliknieta(KartaWidget* kliknietaKarta);
    void zagrajWybranaKarte();
};

#endif // MAINWINDOW_H