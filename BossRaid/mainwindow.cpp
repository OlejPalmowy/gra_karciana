#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QRandomGenerator> // To musisz dodać na górze pliku, żeby mieć losowanie

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    aktualnyGracz = new Gracz("Bohater", 30, 12);
    bossRaidu = new Boss("Mroczny Władca", 100);
    bossRaidu->dodajPancerz(30);
    // --- TO MUSISZ DODAĆ: Ustawiamy maksymalny zakres paska na podstawie MaxHP z logiki Bossa ---
    ui->bossHpBar->setMaximum(bossRaidu->getMaxHp());

    for(int i = 0; i < 4; i++) {
        dodajLosowaKarte();
    }

    for(int i = 0; i < 5; i++) {
        dodajLosowaKarte();
    }

    aktualizujInterfejs();

    // Podłączamy przycisk z mieczami (Atak)
    connect(ui->btnZagraj, &QPushButton::clicked, this, &MainWindow::zagrajWybranaKarte);

    // Podłączamy przycisk Końca Tury
    connect(ui->endTurnButton, &QPushButton::clicked, this, &MainWindow::zakonczTure);

    connect(ui->btnReroll, &QPushButton::clicked, this, &MainWindow::przerolujKarty);

    dodajKomunikatGracza("Walka z Mrocznym Władcą!\nTwoja tura.");
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dodajLosowaKarte()
{
    if (aktualnyGracz->getReka().size() >= 4) return;

    QString sciezkaGrafiki;
    Karta nowaKarta("", 0, 0, 0, 0, 0);

    // NOWOŚĆ: Losujemy liczbę od 0 do 99, żeby ustalić szanse procentowe
    int szansa = QRandomGenerator::global()->bounded(100);
    int losowyIndeks = 0;

    // 15% szans na drop Legendarnej Uzdrowicielki!
    if (szansa < 8) {
        losowyIndeks = 3;
    } else {
        // W przeciwnym razie losujemy standardowo Rycerza, Łucznika lub Maga (0, 1 lub 2)
        losowyIndeks = QRandomGenerator::global()->bounded(3);
    }

    // Wypełniamy dane
    if (losowyIndeks == 0) {
        nowaKarta = Karta("Rycerz", 1, 3, 9, 0, 3);
        sciezkaGrafiki = ":/assets/karta_rycerz.png";
    }
    else if (losowyIndeks == 1) {
        nowaKarta = Karta("Łucznik", 2, 4, 3, 0, 1);
        sciezkaGrafiki = ":/assets/karta_lucznik.png";
    }
    else if (losowyIndeks == 2) {
        nowaKarta = Karta("Mag", 3, 15, 3, 0, 3);
        sciezkaGrafiki = ":/assets/karta_czarodziej.png";
    }
    else if (losowyIndeks == 3) {
        // Uzdrowicielka (Typ = 4). Koszt 0 PA!
        nowaKarta = Karta("Driada", 4, 0, 0, 0, 0);

        // PAMIĘTAJ O PODMIANIE ŚCIEŻKI NA NAZWĘ TWOJEJ GRAFIKI!
        sciezkaGrafiki = ":/assets/karta_driada.png";
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

    widgetyWRece.push_back(nowyWidget);
}

void MainWindow::onKartaKliknieta(KartaWidget* kliknietaKarta)
{
    // Wysyłamy do karty sygnał odwrotny do jej obecnego stanu
    kliknietaKarta->oznaczJakoWybrana(!kliknietaKarta->czyWybrana);
}

void MainWindow::zagrajWybranaKarte()
{
    std::vector<int> indeksyDoUsuniecia;
    int sumaKosztuPA = 0;

    // 1. Szukamy wybranych kart TYLKO w naszej zaufanej liście (od tyłu, aby nie psuć indeksów)
    for(int i = widgetyWRece.size() - 1; i >= 0; i--) {
        if (widgetyWRece[i]->czyWybrana) {
            indeksyDoUsuniecia.push_back(i);
            // Pobieramy koszt PA – teraz indeksy zawsze w 100% się zgadzają!
            sumaKosztuPA += aktualnyGracz->getReka()[i].getKosztPA();
        }
    }

    // Bezpieczniki
    if (indeksyDoUsuniecia.empty()) return;
    if (indeksyDoUsuniecia.size() > 3) return;

    // 2. Jeśli nas stać, odpalamy akcję
    if (aktualnyGracz->zuzyjPA(sumaKosztuPA)) {

        std::vector<Karta> reka = aktualnyGracz->getReka();

        // 3. Odpalamy ataki
        for (int i = indeksyDoUsuniecia.size() - 1; i >= 0; i--) {
            int idx = indeksyDoUsuniecia[i];
            Karta karta = reka[idx];

            int typ = karta.getTyp();
            int dmgHP = karta.getObrazeniaHP();
            int dmgArmor = karta.getObrazeniaPancerz();
            int pancerzBossa = bossRaidu->getPancerz();

            if (typ == 1) { // RYCERZ
                bossRaidu->otrzymajObrazenia(dmgArmor);
                bossRaidu->otrzymajObrazenia(dmgHP);
                aktualnyGracz->dodajPancerz(5);
            }
            else if (typ == 2) { // ŁUCZNIK (Szybki, tani standardowy atak)
                // Po prostu zadaje swoje obrażenia w pancerz, a potem w HP
                bossRaidu->otrzymajObrazenia(dmgArmor);
                bossRaidu->otrzymajObrazenia(dmgHP);
            }
            else if (typ == 3) { // MAG
                bossRaidu->resetujPancerz();
                bossRaidu->otrzymajObrazenia(dmgHP);
                bossRaidu->dodajPancerz(pancerzBossa);
            }
            else if (typ == 4) { // UZDROWICIELKA
                aktualnyGracz->ulecz(10); // Odnawia 10 HP
                dodajKomunikatGracza("Uzdrowicielka leczy Twoje rany! (+10 HP)");
            }
        }

        // 4. Sprzątanie ze stołu i logiki gry
        for (size_t i = 0; i < indeksyDoUsuniecia.size(); i++) {
            int idx = indeksyDoUsuniecia[i];

            // Usuwamy kartę z rąk gracza
            aktualnyGracz->usunKarteZReki(idx);

            // Wyciągamy fizyczny widget z naszej listy i zdejmujemy z okna
            KartaWidget* kw = widgetyWRece[idx];
            ui->handLayout->removeWidget(kw);
            kw->hide();
            kw->deleteLater();

            // Kasujemy wpis oidgecie z naszej listy (żeby rozmiar idealnie zgadzał się z wektorem logiki!)
            widgetyWRece.erase(widgetyWRece.begin() + idx);
        }

        // 5. Dociągamy brakujące karty
        for (size_t i = 0; i < indeksyDoUsuniecia.size(); i++) {
            dodajLosowaKarte();
        }

        // 1. Najpierw sprawdzamy furię Bossa
        if (bossRaidu->getFaza() == 1 && bossRaidu->getHp() <= (bossRaidu->getMaxHp() / 2)) {
            bossRaidu->aktualizujFaze(); // Tutaj boss dostaje matematycznie +15 pancerza

            dodajKomunikatBossa("\"NIE POZWOLĘ SIĘ POKONAĆ, NĘDZNY ROBAKU!!!\"");
            dodajKomunikatGracza("Uważaj! Boss wpada w furię! Zyskał +15 pancerza i bije teraz podwójnie!");
        }

        // 2. DOPIERO TERAZ odświeżamy cały interfejs na ekranie!
        aktualizujInterfejs(); // Ta funkcja teraz pobierze już zaktualizowany, nowy pancerz

        sprawdzKoniecGry();
    }
}

void MainWindow::zakonczTure()
    {
        int typAtaku = QRandomGenerator::global()->bounded(1, 4);

        int obrazeniaOdBossa = bossRaidu->wykonajAtak(typAtaku);
        aktualnyGracz->otrzymajObrazenia(obrazeniaOdBossa);

        aktualnyGracz->odnowPA();

        if (typAtaku == 1) {
            dodajKomunikatBossa("\"Giń, śmiertelniku!\"");
            dodajKomunikatGracza("Rycerz: Mroczny Władca cię atakuje!\n-> Otrzymujesz " + QString::number(obrazeniaOdBossa) + " obrażeń!");
        }
        else if (typAtaku == 2) {
            dodajKomunikatBossa("\"Aaa wy sku***! Oberwiecie za to!\"");
            dodajKomunikatGracza("Rycerz: Demon wpada w furię i ładuje potężny cios!\n-> Otrzymujesz aż " + QString::number(obrazeniaOdBossa) + " obrażeń!");
        }
        else if (typAtaku == 3) {
            dodajKomunikatBossa("\"Mrok mnie chroni!\"");
            dodajKomunikatGracza("Rycerz: Boss ignoruje atak i skupia energię...\n-> Demon odnawia swoją barierę!");
        }

        aktualizujInterfejs();
        sprawdzKoniecGry(obrazeniaOdBossa);
    }

void MainWindow::aktualizujInterfejs()
{
    // Zamieniamy liczby z logiki na tekst i wrzucamy do odpowiednich labeli na ekranie
    ui->bossHpBar->setValue(bossRaidu->getHp());

    // Wyświetlanie aktualnego pancerza Bossa
        ui->bossArmorText->setText(QString::number(bossRaidu->getPancerz()));

    // Twoje statystyki (podpięte pod Twoje nowe labele)
    ui->apText->setText(QString::number(aktualnyGracz->getPA()));
    ui->hpText->setText(QString::number(aktualnyGracz->getHp()));
    ui->armorText->setText(QString::number(aktualnyGracz->getPancerz()));
}

void MainWindow::dodajKomunikatGracza(const QString &tekst)
{
    ui->labelKomunikaty->setText(tekst);
}

void MainWindow::dodajKomunikatBossa(const QString &tekst)
{
    ui->bossText->setText(tekst);
}

void MainWindow::sprawdzKoniecGry(int ostatnieObrazenia)
{
    if (bossRaidu->getHp() <= 0) {
            dodajKomunikatBossa("\"NIEEE! Jak to możliwe...\"");
            dodajKomunikatGracza("ZWYCIĘSTWO!\nMroczny Władca został pokonany!");

            ui->btnZagraj->setEnabled(false);
            ui->endTurnButton->setEnabled(false);
    }
    else if (aktualnyGracz->getHp() <= 0) {
            dodajKomunikatBossa("\"Hahaha! Żałosny robak!\"");

            // ZMIANA: Rycerz melduje o potężnym hicie przed śmiercią
            dodajKomunikatGracza("GAME OVER!\nRycerz: Otrzymaliśmy śmiertelny cios za "
                                 + QString::number(ostatnieObrazenia)
                                 + " HP... Poległeś w nierównej walce.");

        ui->btnZagraj->setEnabled(false);
            ui->endTurnButton->setEnabled(false);
    }
}

void MainWindow::przerolujKarty()
{
    // Reroll kosztuje np. 2 PA
    if (aktualnyGracz->zuzyjPA(2)) {

        // 1. Kasujemy fizyczne widgety z okienka
        for (KartaWidget* kw : widgetyWRece) {
            ui->handLayout->removeWidget(kw);
            kw->hide();
            kw->deleteLater();
        }
        widgetyWRece.clear(); // Opróżniamy naszą listę pomocniczą

        // 2. Czyścimy twardą logikę gracza
        aktualnyGracz->wyczyscReke();

        // 3. Dociągamy 4 nowe karty z szansą na leczenie
        for (int i = 0; i < 4; i++) {
            dodajLosowaKarte();
        }

        aktualizujInterfejs();
        dodajKomunikatGracza("Wymieniłeś wszystkie karty! (-2 PA)");
    } else {
        dodajKomunikatGracza("Brak PA na wymianę kart!");
    }
}