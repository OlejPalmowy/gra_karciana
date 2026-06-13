#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "Karta.h"
#include <QRandomGenerator>
#include <QTimer> // Biblioteka do obsługi opóźnień czasowych (serii dźwięków)

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->labelKomunikaty->setWordWrap(true);
    ui->bossText->setWordWrap(true);

    aktualnyGracz = std::make_unique<Gracz>("Bohater", 30, 12);
    bossRaidu = std::make_unique<Boss>("Mroczny Władca", 100);

    bossRaidu->dodajPancerz(30);
    ui->bossHpBar->setMaximum(bossRaidu->getMaxHp());

    // --- INICJALIZACJA DŹWIĘKÓW (Z POJEDYNCZYM .wav) ---
    sfxMiecz = new QSoundEffect(this);
    sfxMiecz->setSource(QUrl("qrc:/assets/miecz.wav"));

    sfxStrzala = new QSoundEffect(this);
    sfxStrzala->setSource(QUrl("qrc:/assets/strzala.wav"));

    sfxMagia = new QSoundEffect(this);
    sfxMagia->setSource(QUrl("qrc:/assets/magia.wav"));

    sfxDriada = new QSoundEffect(this);
    sfxDriada->setSource(QUrl("qrc:/assets/driada.wav")); // DODANE: Ścieżka do dźwięku Driady

    sfxWygrana = new QSoundEffect(this);
    sfxWygrana->setSource(QUrl("qrc:/assets/wygrana.wav"));

    sfxPrzegrana = new QSoundEffect(this);
    sfxPrzegrana->setSource(QUrl("qrc:/assets/przegrana.wav"));

    for(int i = 0; i < 4; i++) dodajLosowaKarte();

    aktualizujInterfejs();

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
    std::shared_ptr<Karta> nowaKarta;

    int szansa = QRandomGenerator::global()->bounded(100);
    int losowyIndeks = 0;

    if (szansa < 8) losowyIndeks = 3;
    else losowyIndeks = QRandomGenerator::global()->bounded(3);

    if (losowyIndeks == 0) {
        nowaKarta = std::make_shared<KartaRycerz>();
        sciezkaGrafiki = ":/assets/karta_rycerz.png";
        opisKarty = "Zadaje 9 pancerza LUB 3 HP (gdy brak tarczy). Daje +5 pancerza graczowi.";
    }
    else if (losowyIndeks == 1) {
        nowaKarta = std::make_shared<KartaLucznik>();
        sciezkaGrafiki = ":/assets/karta_lucznik.png";
        opisKarty = "Zadaje 3 pancerza LUB 4 HP (gdy brak tarczy).";
    }
    else if (losowyIndeks == 2) {
        nowaKarta = std::make_shared<KartaMag>();
        sciezkaGrafiki = ":/assets/karta_czarodziej.png";
        opisKarty = "Całkowicie omija pancerz i bije prosto w zdrowie bossa.";
    }
    else if (losowyIndeks == 3) {
        nowaKarta = std::make_shared<KartaDriada>();
        sciezkaGrafiki = ":/assets/karta_driada.png";
        opisKarty = "Mityczna driada. Uzdrawia gracza z odniesionych ran (+10 HP).";
    }

    aktualnyGracz->dodajKarteDoReki(nowaKarta);

    KartaWidget *nowyWidget = new KartaWidget(this);
    nowyWidget->ustawGrafike(sciezkaGrafiki);
    nowyWidget->ustawStatystyki(nowaKarta->getObrazeniaHP(), nowaKarta->getKosztPA(), opisKarty);

    connect(nowyWidget, &KartaWidget::kartaKliknieta, this, &MainWindow::onKartaKliknieta);
    ui->handLayout->addWidget(nowyWidget);
    widgetyWRece.push_back(nowyWidget);
}

void MainWindow::onKartaKliknieta(KartaWidget* kliknietaKarta)
{
    kliknietaKarta->oznaczJakoWybrana(!kliknietaKarta->getCzyWybrana());
}

void MainWindow::zagrajWybranaKarte()
{
    std::vector<int> indeksyDoUsuniecia;
    int sumaKosztuPA = 0;

    for(int i = widgetyWRece.size() - 1; i >= 0; i--) {
        if (widgetyWRece[i]->getCzyWybrana()) {
            indeksyDoUsuniecia.push_back(i);
            sumaKosztuPA += aktualnyGracz->getReka()[i]->getKosztPA();
        }
    }

    if (indeksyDoUsuniecia.empty() || indeksyDoUsuniecia.size() > 3) return;

    if (aktualnyGracz->zuzyjPA(sumaKosztuPA)) {

        std::vector<std::shared_ptr<Karta>> reka = aktualnyGracz->getReka();

        // Pętla wykonująca ataki kart (odpalana od tyłu)
        for (int i = indeksyDoUsuniecia.size() - 1; i >= 0; i--) {
            int idx = indeksyDoUsuniecia[i];
            std::shared_ptr<Karta> karta = reka[idx];

            // 1. Logika walki (wykonuje się natychmiast)
            QString odpKarty = karta->zagraj(bossRaidu.get(), aktualnyGracz.get());

            // 2. Obliczanie przerw czasowych - ZWIĘKSZONO MNOŻNIK Z 200 NA 400 dla większego odstępu
            int opoznienie = (indeksyDoUsuniecia.size() - 1 - i) * 400;
            int typKarty = karta->getTyp();

            // 3. Budzik odtwarzający dźwięk po określonym czasie z trikiem Windowsa
            QTimer::singleShot(opoznienie, this, [this, typKarty]() {
                if (typKarty == 1) {
                    sfxMiecz->setSource(sfxMiecz->source());
                    sfxMiecz->play();
                }
                else if (typKarty == 2) {
                    sfxStrzala->setSource(sfxStrzala->source());
                    sfxStrzala->play();
                }
                else if (typKarty == 3) {
                    sfxMagia->setSource(sfxMagia->source());
                    sfxMagia->play();
                }
                else if (typKarty == 4) { // POPRAWIONE: Dodana obsługa dźwięku dla Driady
                    sfxDriada->setSource(sfxDriada->source());
                    sfxDriada->play();
                }
            });

            if (!odpKarty.isEmpty()) {
                dodajKomunikatGracza(odpKarty);
            }
        }

        for (size_t i = 0; i < indeksyDoUsuniecia.size(); i++) {
            int idx = indeksyDoUsuniecia[i];
            aktualnyGracz->usunKarteZReki(idx);
            KartaWidget* kw = widgetyWRece[idx];
            ui->handLayout->removeWidget(kw);
            kw->hide();
            kw->deleteLater();
            widgetyWRece.erase(widgetyWRece.begin() + idx);
        }

        for (size_t i = 0; i < indeksyDoUsuniecia.size(); i++) dodajLosowaKarte();

        if (bossRaidu->getFaza() == 1 && bossRaidu->getHp() <= (bossRaidu->getMaxHp() / 2)) {
            bossRaidu->aktualizujFaze();
            dodajKomunikatBossa("\"NIE POZWOLĘ SIĘ POKONAĆ, NĘDZNY ROBAKU!!!\"");
            dodajKomunikatGracza("Uważaj! Boss wpada w furię! Zyskał +15 pancerza i bije teraz podwójnie!");
        }

        aktualizujInterfejs();
        sprawdzKoniecGry();
    }
}

void MainWindow::zakonczTure()
{
    int typAtaku = QRandomGenerator::global()->bounded(1, 4);

    int obrazeniaOdBossa = bossRaidu->wykonajAtak(typAtaku);
    aktualnyGracz->otrzymajObrazenia(obrazeniaOdBossa);

    aktualnyGracz->przygotujDoTury();
    bossRaidu->przygotujDoTury();

    bool czyFuria = (bossRaidu->getFaza() == 2);
    if (typAtaku == 1) {
        if (czyFuria) {
            dodajKomunikatBossa("\"ZGINIECIE W MĘCZARNIACH!!!\"");
            dodajKomunikatGracza("Rycerz: Uważaj! Wściekły boss wyprowadza szybkie cięcie! (-" + QString::number(obrazeniaOdBossa) + " HP)");
        } else {
            dodajKomunikatBossa("\"Giń, śmiertelniku!\"");
            dodajKomunikatGracza("Rycerz: Boss wykonuje zwykłe cięcie. (-" + QString::number(obrazeniaOdBossa) + " HP)");
        }
    }
    else if (typAtaku == 2) {
        if (czyFuria) {
            dodajKomunikatBossa("\"POCZUJCIE MOJĄ PRAWDZIWĄ POTĘGĘ!!!\"");
            dodajKomunikatGracza("Rycerz: KRYTYK! Rozwścieczony demon miażdży nas potężnym uderzeniem! (-" + QString::number(obrazeniaOdBossa) + " HP)");
        } else {
            dodajKomunikatBossa("\"Poczuj gniew otchłani!\"");
            dodajKomunikatGracza("Rycerz: Uważaj! Boss ładuje mocne uderzenie. (-" + QString::number(obrazeniaOdBossa) + " HP)");
        }
    }
    else if (typAtaku == 3) {
        dodajKomunikatBossa("\"Mrok mnie leczy i chroni!\"");
        dodajKomunikatGracza("Rycerz: Boss odnawia barierę i zasklepia swoje rany! (+15 Pancerza, +15 HP)");
    }

    aktualizujInterfejs();
    sprawdzKoniecGry(obrazeniaOdBossa);
}

void MainWindow::aktualizujInterfejs()
{
    ui->bossHpBar->setValue(bossRaidu->getHp());
    ui->bossArmorText->setText(QString::number(bossRaidu->getPancerz()));
    ui->bossHpText->setText(QString::number(bossRaidu->getHp()) + " / " + QString::number(bossRaidu->getMaxHp()));

    ui->apText->setText(QString::number(aktualnyGracz->getPA()));
    ui->hpText->setText(QString::number(aktualnyGracz->getHp()));
    ui->armorText->setText(QString::number(aktualnyGracz->getPancerz()));

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
        sfxWygrana->play(); // Odpalenie fanfar po zwycięstwie

        dodajKomunikatBossa("\"NIEEE! Jak to możliwe...\"");
        dodajKomunikatGracza("ZWYCIĘSTWO!\nMroczny Władca został pokonany!");
        ui->btnZagraj->setEnabled(false);
        ui->endTurnButton->setEnabled(false);
    }
    else if (aktualnyGracz->getHp() <= 0) {
        sfxPrzegrana->play(); // Odpalenie ponurego dźwięku po przegranej

        dodajKomunikatBossa("\"Hahaha! Żałosny robak!\"");
        dodajKomunikatGracza("GAME OVER!\nRycerz: Otrzymaliśmy śmiertelny cios za "
                             + QString::number(ostatnieObrazenia)
                             + " HP... Poległeś w nierównej walce.");
        ui->btnZagraj->setEnabled(false);
        ui->endTurnButton->setEnabled(false);
    }
}

void MainWindow::przerolujKarty()
{
    if (aktualnyGracz->zuzyjPA(2)) {
        for (KartaWidget* kw : widgetyWRece) {
            ui->handLayout->removeWidget(kw);
            kw->hide();
            kw->deleteLater();
        }
        widgetyWRece.clear();
        aktualnyGracz->wyczyscReke();
        for (int i = 0; i < 4; i++) dodajLosowaKarte();
        aktualizujInterfejs();
        dodajKomunikatGracza("Wymieniłeś wszystkie karty! (-2 PA)");
    } else {
        dodajKomunikatGracza("Brak PA na wymianę kart!");
    }
}