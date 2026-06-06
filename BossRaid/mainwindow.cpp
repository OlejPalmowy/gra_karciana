#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QRandomGenerator> // To musisz dodać na górze pliku, żeby mieć losowanie

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    aktualnyGracz = new Gracz("Bohater", 30, 18);
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

    dodajKomunikat("Walka z Mrocznym Władcą!\nTwoja tura.");
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
    // 1. Zbieramy wszystkie podniesione karty
    // Używamy odwróconej pętli, bo jak potem będziemy usuwać karty z ręki,
    // usunięcie pierwszej zmieniłoby numery pozostałych. Usuwając od końca - omijamy problem.
    std::vector<int> indeksyDoUsuniecia;
    std::vector<KartaWidget*> widgetyDoUsuniecia;
    int sumaKosztuPA = 0;

    for(int i = ui->handLayout->count() - 1; i >= 0; i--) {
        KartaWidget* kw = qobject_cast<KartaWidget*>(ui->handLayout->itemAt(i)->widget());
        if (kw != nullptr && kw->czyWybrana) {
            indeksyDoUsuniecia.push_back(i);
            widgetyDoUsuniecia.push_back(kw);
            // Zbieramy całkowity koszt PA tego combosa
            sumaKosztuPA += aktualnyGracz->getReka()[i].getKosztPA();
        }
    }

    // 2. Bezpieczniki: czy cokolwiek wybrano i czy nie przekroczono limitu 3 kart
    if (indeksyDoUsuniecia.empty()) return;
    if (indeksyDoUsuniecia.size() > 3) {
        // Tu docelowo można dać komunikat "Maksymalnie 3 karty!", na razie po prostu przerywamy
        return;
    }

    // 3. Sprawdzamy, czy stać nas na całą akcję naraz
    if (aktualnyGracz->zuzyjPA(sumaKosztuPA)) {

        std::vector<Karta> reka = aktualnyGracz->getReka();

        // 4. Odpalamy ataki (odwracamy pętlę z powrotem, żeby ataki szły od lewej do prawej)
        for (int i = indeksyDoUsuniecia.size() - 1; i >= 0; i--) {
            Karta karta = reka[indeksyDoUsuniecia[i]];

            int typ = karta.getTyp();
            int dmgHP = karta.getObrazeniaHP();
            int dmgArmor = karta.getObrazeniaPancerz();
            int pancerzBossa = bossRaidu->getPancerz();

            if (typ == 1) { // RYCERZ
                bossRaidu->otrzymajObrazenia(dmgArmor);
                bossRaidu->otrzymajObrazenia(dmgHP);
                aktualnyGracz->dodajPancerz(5);
            }
            else if (typ == 2) { // ŁUCZNIK
                if (pancerzBossa > 0) {
                    if (dmgArmor > pancerzBossa) {
                        int nadmiar = dmgArmor - pancerzBossa;
                        int bonusDmgHP = nadmiar / 2;
                        bossRaidu->otrzymajObrazenia(pancerzBossa);
                        bossRaidu->otrzymajObrazenia(dmgHP + bonusDmgHP);
                    } else {
                        bossRaidu->otrzymajObrazenia(dmgArmor);
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
        }

        // 5. Sprzątanie: usuwamy wszystkie rzucone karty ze stołu
        for (size_t i = 0; i < indeksyDoUsuniecia.size(); i++) {
            aktualnyGracz->usunKarteZReki(indeksyDoUsuniecia[i]);
            KartaWidget* kw = widgetyDoUsuniecia[i];
            ui->handLayout->removeWidget(kw);
            kw->hide();
            kw->deleteLater();
        }

        // 6. Dociągamy dokładnie tyle nowych kart, ile wyrzuciliśmy
        for (size_t i = 0; i < indeksyDoUsuniecia.size(); i++) {
            dodajLosowaKarte();
        }

        aktualizujInterfejs(); // Upewniamy się, że HP, Pancerz i PA od razu przeskoczą na ekranie
        sprawdzKoniecGry();
    }
}

void MainWindow::zakonczTure()
{
    dodajKomunikat("Koniec Twojej tury. Atakuje Boss!");

    // Boss wykonuje atak, a Twój gracz dostaje obrażenia (pancerz zredukuje je automatycznie)
    int obrazeniaOdBossa = bossRaidu->wykonajAtak();
    aktualnyGracz->otrzymajObrazenia(obrazeniaOdBossa);

    // Odnawiamy Twoje Punkty Akcji
    aktualnyGracz->resetujPA();

    // Rycerzyk raportuje, ile oberwaliśmy
    dodajKomunikat("Boss zadaje " + QString::number(obrazeniaOdBossa) + " obrażeń!\nTwoja tura.");

    // Aktualizujemy wszystko na ekranie
    aktualizujInterfejs();
    sprawdzKoniecGry();
}

void MainWindow::aktualizujInterfejs()
{
    // Zamieniamy liczby z logiki na tekst i wrzucamy do odpowiednich labeli na ekranie
    ui->bossHpBar->setValue(bossRaidu->getHp());

    // Wyświetlanie aktualnego pancerza Bossa
        ui->labelBossPancerz->setText(QString::number(bossRaidu->getPancerz()));

    // Twoje statystyki (podpięte pod Twoje nowe labele)
    ui->apText->setText(QString::number(aktualnyGracz->getPA()));
    ui->hpText->setText(QString::number(aktualnyGracz->getHp()));
    ui->armorText->setText(QString::number(aktualnyGracz->getPancerz()));
}

void MainWindow::dodajKomunikat(const QString &tekst)
{
    ui->labelKomunikaty->setText(tekst);
}

void MainWindow::sprawdzKoniecGry()
{
    if (bossRaidu->getHp() <= 0) {
        dodajKomunikat("ZWYCIĘSTWO!\nMroczny Władca został pokonany!");

        // Blokujemy przyciski
        ui->btnZagraj->setEnabled(false);
        ui->endTurnButton->setEnabled(false);
    }
    else if (aktualnyGracz->getHp() <= 0) {
        dodajKomunikat("GAME OVER!\nPoległeś w nierównej walce...");

        // Blokujemy przyciski
        ui->btnZagraj->setEnabled(false);
        ui->endTurnButton->setEnabled(false);
    }
}