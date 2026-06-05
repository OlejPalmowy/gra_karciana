#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QRandomGenerator> // To musisz dodać na górze pliku, żeby mieć losowanie

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    for(int i = 0; i < 5; i++) {
        dodajLosowaKarte();
}
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dodajLosowaKarte()
{
    // 1. Sprawdzamy limit – jeśli w layoutcie jest już 4 lub więcej elementów, przerywamy
    if (ui->handLayout->count() >= 4) {
        return;
    }

    // 2. Tworzymy tablicę ze ścieżkami do Twoich 3 wyciętych grafik w zasobach .qrc
    // Upewnij się, że nazwy plików w cudzysłowach zgadzają się z Twoimi na dysku!
    QStringList teksturyKart;
    teksturyKart << ":/assets/karta_rycerz.png"
                 << ":/assets/karta_lucznik.png"
                 << ":/assets/karta_czarodziej.png";

    // 3. Losujemy indeks od 0 do 2
    int losowyIndeks = QRandomGenerator::global()->bounded(3);
    QString wylosowanaTekstura = teksturyKart[losowyIndeks];

    // 4. Tworzymy nowy obiekt naszej karty
    KartaWidget *nowaKarta = new KartaWidget(this);

    // 5. Ustawiamy jej wylosowane tło (korzystając z funkcji, którą wcześniej napisaliśmy)
    nowaKarta->ustawGrafike(wylosowanaTekstura);

    // --- TO JEST KLUCZOWE: ŁĄCZYMY SYGNAŁ KARTY Z FUNKCJĄ W MAINWINDOW ---
    connect(nowaKarta, &KartaWidget::kartaKliknieta, this, &MainWindow::onKartaKliknieta);
    // --------------------------------------------------------------------

    // 6. Wrzucamy gotowy widget do przygotowanego przez kolegę handLayout
    ui->handLayout->addWidget(nowaKarta);
}
void MainWindow::onKartaKliknieta(KartaWidget* kliknietaKarta)
{
    // Wysyłamy do karty sygnał odwrotny do jej obecnego stanu
    kliknietaKarta->oznaczJakoWybrana(!kliknietaKarta->czyWybrana);
}