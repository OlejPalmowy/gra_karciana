#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->labelKomunikaty->setWordWrap(true);
    ui->bossText->setWordWrap(true);

    aktualnyGracz = new Gracz("Bohater", 30, 12);
    bossRaidu = new Boss("Mroczny Władca", 100);
    bossRaidu->dodajPancerz(30);

    // ustawienie max hp paska bossa
    ui->bossHpBar->setMaximum(bossRaidu->getMaxHp());

    for(int i = 0; i < 4; i++) {
        dodajLosowaKarte();
    }

    for(int i = 0; i < 5; i++) {
        dodajLosowaKarte();
    }

    aktualizujInterfejs();

    // podpiecie przyciskow
    connect(ui->btnZagraj, &QPushButton::clicked, this, &MainWindow::zagrajWybranaKarte);
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
    QString opisKarty;
    Karta nowaKarta("", 0, 0, 0, 0, 0);

    // losowanie szansy (0-99)
    int szansa = QRandomGenerator::global()->bounded(100);
    int losowyIndeks = 0;

    // szansa na uzdrowicielke
    if (szansa < 8) {
        losowyIndeks = 3;
    } else {
        // losowanie reszty kart (rycerz, lucznik, mag)
        losowyIndeks = QRandomGenerator::global()->bounded(3);
    }

    // przypisanie statystyk
    if (losowyIndeks == 0) {
        nowaKarta = Karta("Rycerz", 1, 3, 9, 0, 3);
        sciezkaGrafiki = ":/assets/karta_rycerz.png";
        opisKarty = "Zadaje 9 pancerza LUB 3 HP (gdy brak tarczy). Daje +5 pancerza graczowi.";
    }
    else if (losowyIndeks == 1) {
        nowaKarta = Karta("Łucznik", 2, 4, 3, 0, 1);
        sciezkaGrafiki = ":/assets/karta_lucznik.png";
        opisKarty = "Zadaje 3 pancerza LUB 4 HP (gdy brak tarczy).";
    }
    else if (losowyIndeks == 2) {
        nowaKarta = Karta("Mag", 3, 15, 3, 0, 3);
        sciezkaGrafiki = ":/assets/karta_czarodziej.png";
        opisKarty = "Całkowicie omija pancerz i bije prosto w zdrowie bossa.";
    }
    else if (losowyIndeks == 3) {
        // uzdrowicielka
        nowaKarta = Karta("Driada", 4, 0, 0, 0, 0);
        sciezkaGrafiki = ":/assets/karta_driada.png";
        opisKarty = "Mityczna driada. Uzdrawia gracza z odniesionych ran (+10 HP).";
    }

    // dodanie karty do logiki
    aktualnyGracz->dodajKarteDoReki(nowaKarta);

    // tworzenie widgetu
    KartaWidget *nowyWidget = new KartaWidget(this);
    nowyWidget->ustawGrafike(sciezkaGrafiki);

    // ustawienie tekstow
    nowyWidget->ustawStatystyki(nowaKarta.getObrazeniaHP(), nowaKarta.getKosztPA(), opisKarty);

    connect(nowyWidget, &KartaWidget::kartaKliknieta, this, &MainWindow::onKartaKliknieta);
    ui->handLayout->addWidget(nowyWidget);
    widgetyWRece.push_back(nowyWidget);
}

void MainWindow::onKartaKliknieta(KartaWidget* kliknietaKarta)
{
    // zaznaczanie/odznaczanie karty
    kliknietaKarta->oznaczJakoWybrana(!kliknietaKarta->czyWybrana);
}

void MainWindow::zagrajWybranaKarte()
{
    std::vector<int> indeksyDoUsuniecia;
    int sumaKosztuPA = 0;

    // szukanie od tylu zeby nie zepsuc indeksow
    for(int i = widgetyWRece.size() - 1; i >= 0; i--) {
        if (widgetyWRece[i]->czyWybrana) {
            indeksyDoUsuniecia.push_back(i);
            sumaKosztuPA += aktualnyGracz->getReka()[i].getKosztPA();
        }
    }

    // blokady
    if (indeksyDoUsuniecia.empty()) return;
    if (indeksyDoUsuniecia.size() > 3) return;

    // sprawdzanie pa
    if (aktualnyGracz->zuzyjPA(sumaKosztuPA)) {

        std::vector<Karta> reka = aktualnyGracz->getReka();

        // ataki
        for (int i = indeksyDoUsuniecia.size() - 1; i >= 0; i--) {
            int idx = indeksyDoUsuniecia[i];
            Karta karta = reka[idx];

            int typ = karta.getTyp();
            int dmgHP = karta.getObrazeniaHP();
            int dmgArmor = karta.getObrazeniaPancerz();
            int pancerzBossa = bossRaidu->getPancerz();

            if (typ == 1) { // RYCERZ
                int pancerzBossa = bossRaidu->getPancerz();
                if (pancerzBossa > 0) {
                    // bije w tarcze
                    if (pancerzBossa >= dmgArmor) {
                        bossRaidu->otrzymajObrazenia(dmgArmor);
                    } else {
                        // zeruje pancerz
                        bossRaidu->otrzymajObrazenia(pancerzBossa);
                    }
                } else {
                    // bije w hp
                    bossRaidu->otrzymajObrazenia(dmgHP);
                }
                aktualnyGracz->dodajPancerz(5);
            }
            else if (typ == 2) { // ŁUCZNIK
                int pancerzBossa = bossRaidu->getPancerz();
                if (pancerzBossa > 0) {
                    if (pancerzBossa >= dmgArmor) {
                        bossRaidu->otrzymajObrazenia(dmgArmor);
                    } else {
                        bossRaidu->otrzymajObrazenia(pancerzBossa);
                    }
                } else {
                    bossRaidu->otrzymajObrazenia(dmgHP);
                }
            }
            else if (typ == 3) { // MAG
                bossRaidu->resetujPancerz();
                bossRaidu->otrzymajObrazenia(dmgHP);
                bossRaidu->dodajPancerz(pancerzBossa);
            }
            else if (typ == 4) { // UZDROWICIELKA
                aktualnyGracz->ulecz(10);
                dodajKomunikatGracza("Uzdrowicielka leczy Twoje rany! (+10 HP)");
            }
        }

        // usuwanie zagranych kart
        for (size_t i = 0; i < indeksyDoUsuniecia.size(); i++) {
            int idx = indeksyDoUsuniecia[i];

            // usuwanie z reki
            aktualnyGracz->usunKarteZReki(idx);

            // usuwanie widgetu z ekranu
            KartaWidget* kw = widgetyWRece[idx];
            ui->handLayout->removeWidget(kw);
            kw->hide();
            kw->deleteLater();

            // wyrzucenie z listy
            widgetyWRece.erase(widgetyWRece.begin() + idx);
        }

        // dobieranie nowych kart
        for (size_t i = 0; i < indeksyDoUsuniecia.size(); i++) {
            dodajLosowaKarte();
        }

        // sprawdzenie furii
        if (bossRaidu->getFaza() == 1 && bossRaidu->getHp() <= (bossRaidu->getMaxHp() / 2)) {
            bossRaidu->aktualizujFaze();
            dodajKomunikatBossa("\"NIE POZWOLĘ SIĘ POKONAĆ, NĘDZNY ROBAKU!!!\"");
            dodajKomunikatGracza("Uważaj! Boss wpada w furię! Zyskał +15 pancerza i bije teraz podwójnie!");
        }

        // update ui
        aktualizujInterfejs();
        sprawdzKoniecGry();
    }
}

void MainWindow::zakonczTure()
{
    int typAtaku = QRandomGenerator::global()->bounded(1, 4);

    int obrazeniaOdBossa = bossRaidu->wykonajAtak(typAtaku);
    aktualnyGracz->otrzymajObrazenia(obrazeniaOdBossa);

    aktualnyGracz->odnowPA();

    // czy boss ma furie
    bool czyFuria = (bossRaidu->getFaza() == 2);

    if (typAtaku == 1) { // zwykly atak
        if (czyFuria) {
            dodajKomunikatBossa("\"ZGINIECIE W MĘCZARNIACH!!!\"");
            dodajKomunikatGracza("Rycerz: Uważaj! Wściekły boss wyprowadza szybkie cięcie! (-" + QString::number(obrazeniaOdBossa) + " HP)");
        } else {
            dodajKomunikatBossa("\"Giń, śmiertelniku!\"");
            dodajKomunikatGracza("Rycerz: Boss wykonuje zwykłe cięcie. (-" + QString::number(obrazeniaOdBossa) + " HP)");
        }
    }
    else if (typAtaku == 2) { // silny atak
        if (czyFuria) {
            dodajKomunikatBossa("\"POCZUJCIE MOJĄ PRAWDZIWĄ POTĘGĘ!!!\"");
            dodajKomunikatGracza("Rycerz: KRYTYK! Rozwścieczony demon miażdży nas potężnym uderzeniem! (-" + QString::number(obrazeniaOdBossa) + " HP)");
        } else {
            dodajKomunikatBossa("\"Poczuj gniew otchłani!\"");
            dodajKomunikatGracza("Rycerz: Uważaj! Boss ładuje mocne uderzenie. (-" + QString::number(obrazeniaOdBossa) + " HP)");
        }
    }
    else if (typAtaku == 3) { // leczenie bossa
        dodajKomunikatBossa("\"Mrok mnie leczy i chroni!\"");
        dodajKomunikatGracza("Rycerz: Boss odnawia barierę i zasklepia swoje rany! (+15 Pancerza, +15 HP)");
    }

    aktualizujInterfejs();
    sprawdzKoniecGry(obrazeniaOdBossa);
}

void MainWindow::aktualizujInterfejs()
{
    // update wartosci na ui
    ui->bossHpBar->setValue(bossRaidu->getHp());
    ui->bossArmorText->setText(QString::number(bossRaidu->getPancerz()));
    ui->bossHpText->setText(QString::number(bossRaidu->getHp()) + " / " + QString::number(bossRaidu->getMaxHp()));

    // staty gracza
    ui->apText->setText(QString::number(aktualnyGracz->getPA()));
    ui->hpText->setText(QString::number(aktualnyGracz->getHp()));
    ui->armorText->setText(QString::number(aktualnyGracz->getPancerz()));

    // kolorki
    ui->bossHpText->setStyleSheet("color: #A30000; font-weight: bold;");
    ui->hpText->setStyleSheet("color: #FF2E2E; font-weight: bold;");
    ui->apText->setStyleSheet("color: #2EB1FF; font-weight: bold;");
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

        // info o smierci
        dodajKomunikatGracza("GAME OVER!\nRycerz: Otrzymaliśmy śmiertelny cios za "
                             + QString::number(ostatnieObrazenia)
                             + " HP... Poległeś w nierównej walce.");

        ui->btnZagraj->setEnabled(false);
        ui->endTurnButton->setEnabled(false);
    }
}

void MainWindow::przerolujKarty()
{
    // koszt rerolla = 2 PA
    if (aktualnyGracz->zuzyjPA(2)) {

        // czyszczenie ui z kart
        for (KartaWidget* kw : widgetyWRece) {
            ui->handLayout->removeWidget(kw);
            kw->hide();
            kw->deleteLater();
        }
        widgetyWRece.clear();

        // czyszczenie reki w logice
        aktualnyGracz->wyczyscReke();

        // dobieranie 4 kart
        for (int i = 0; i < 4; i++) {
            dodajLosowaKarte();
        }

        aktualizujInterfejs();
        dodajKomunikatGracza("Wymieniłeś wszystkie karty! (-2 PA)");
    } else {
        dodajKomunikatGracza("Brak PA na wymianę kart!");
    }
}