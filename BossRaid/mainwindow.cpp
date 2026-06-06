#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QRandomGenerator> // To musisz dodać na górze pliku, żeby mieć losowanie

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    aktualnyGracz = new Gracz("Bohater", 30, 3);
    bossRaidu = new Boss("Mroczny Władca", 100);

    // --- TO MUSISZ DODAĆ: Ustawiamy maksymalny zakres paska na podstawie MaxHP z logiki Bossa ---
    ui->bossHpBar->setMaximum(bossRaidu->getMaxHp());

    for(int i = 0; i < 4; i++) {
        dodajLosowaKarte();
    }

    for(int i = 0; i < 5; i++) {
        dodajLosowaKarte();
    }

    aktualizujInterfejs();

    // Podłączamy Twój przycisk do funkcji zagrywania karty
    connect(ui->endTurnButton, &QPushButton::clicked, this, &MainWindow::zagrajWybranaKarte);
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dodajLosowaKarte()
{
    if (ui->handLayout->count() >= 4) return;

    // 1. Losujemy jaka to ma być karta (0 = Rycerz, 1 = Łucznik, 2 = Mag)
    int losowyIndeks = QRandomGenerator::global()->bounded(3);

    // Zmienne pomocnicze - przygotowujemy pustą ścieżkę i pustą kartę
    QString sciezkaGrafiki;
    Karta nowaKarta("", 0, 0, 0, 0, 0);

    // 2. Wypełniamy dane w zależności od tego, co się wylosowało
    if (losowyIndeks == 0) {
        // Rycerz: 3 obrażeń w HP, 9 w pancerz, koszt 1 PA
        nowaKarta = Karta("Rycerz", 1, 3, 9, 0, 1);
        sciezkaGrafiki = ":/assets/karta_rycerz.png";
    }
    else if (losowyIndeks == 1) {
        // Łucznik: 6 obrażeń w HP, 6 w pancerz, koszt 2 PA
        nowaKarta = Karta("Łucznik", 2, 6, 6, 0, 2);
        sciezkaGrafiki = ":/assets/karta_lucznik.png";
    }
    else if (losowyIndeks == 2) {
        // Mag: 12 obrażeń w HP, 3 w pancerz, koszt 3 PA
        nowaKarta = Karta("Mag", 3, 12, 3, 0, 3);
        sciezkaGrafiki = ":/assets/karta_czarodziej.png";
    }

    // 3. Dodajemy gotową, wylosowaną kartę do logiki gracza
    aktualnyGracz->dodajKarteDoReki(nowaKarta);

    // 4. Tworzymy widok (widget) i podpinamy grafikę
    KartaWidget *nowyWidget = new KartaWidget(this);
    nowyWidget->ustawGrafike(sciezkaGrafiki);

    // 5. Przekazujemy statystyki z naszej nowej karty na ekran
    // Używamy getObrazeniaHP() - możesz potem to zmienić, jeśli na lewym polu wolisz inny wskaźnik
    nowyWidget->ustawStatystyki(nowaKarta.getObrazeniaHP(), nowaKarta.getKosztPA());

    connect(nowyWidget, &KartaWidget::kartaKliknieta, this, &MainWindow::onKartaKliknieta);
    ui->handLayout->addWidget(nowyWidget);
}

void MainWindow::onKartaKliknieta(KartaWidget* kliknietaKarta)
{
    // Wysyłamy do karty sygnał odwrotny do jej obecnego stanu
    kliknietaKarta->oznaczJakoWybrana(!kliknietaKarta->czyWybrana);
}

void MainWindow::zagrajWybranaKarte()
{
    // 1. Szukamy, która karta jest zaznaczona (ma flagę czyWybrana == true)
    int indeksWybranej = -1;
    KartaWidget* wybranyWidget = nullptr;

    for(int i = 0; i < ui->handLayout->count(); i++) {
        // qobject_cast pozwala nam bezpiecznie wyciągnąć Twój KartaWidget z layoutu
        KartaWidget* kw = qobject_cast<KartaWidget*>(ui->handLayout->itemAt(i)->widget());
        if (kw != nullptr && kw->czyWybrana == true) {
            indeksWybranej = i;
            wybranyWidget = kw;
            break; // Znaleźliśmy podniesioną kartę, przerywamy szukanie
        }
    }

    // Jeśli żadna nie jest w górze, przerywamy akcję (nic się nie dzieje)
    if (indeksWybranej == -1) return;

    // 2. Pobieramy dane wybranej karty z Twojego wektora "reka" z klasy Gracz
    std::vector<Karta> rekaGracza = aktualnyGracz->getReka();
    Karta kartaDoZagrania = rekaGracza[indeksWybranej];

    // 3. Sprawdzamy, czy masz wystarczająco Punktów Akcji (funkcja zuzyjPA() sama je odejmie, jeśli tak)
    if (aktualnyGracz->zuzyjPA(kartaDoZagrania.getKosztPA())) {

        // 4. Zadajemy obrażenia Bossowi!
        // Na ten moment wrzucamy prostą sumę. Bardziej zaawansowaną logikę przebijania pancerza dopiszemy w następnym kroku.
        int sumaObrazen = kartaDoZagrania.getObrazeniaHP() + kartaDoZagrania.getObrazeniaPancerz();
        bossRaidu->otrzymajObrazenia(sumaObrazen);

        // 5. Sprzątanie w logice: usuwamy tę kartę z wektora Gracza
        aktualnyGracz->usunKarteZReki(indeksWybranej);

        // 6. Sprzątanie na ekranie (ZMIANA TUTAJ)
        ui->handLayout->removeWidget(wybranyWidget); // Wyrywamy kartę z layoutu natychmiast
        wybranyWidget->hide();                       // Chowamy ją, by zniknęła z oczu
        wybranyWidget->deleteLater();                // Leniwe usunięcie z pamięci

        // 7. Od razu dobieramy nową kartę na jej miejsce!
        dodajLosowaKarte();

    } else {
        // Tutaj w przyszłości można wyświetlić np. czerwony napis "Brak Punktów Akcji!" na ekranie
    }

    aktualizujInterfejs();
}

void MainWindow::aktualizujInterfejs()
{
    // Zamieniamy liczby z logiki na tekst i wrzucamy do odpowiednich labeli na ekranie
    ui->bossHpBar->setValue(bossRaidu->getHp());
    ui->apText->setText(QString::number(aktualnyGracz->getPA()));
}