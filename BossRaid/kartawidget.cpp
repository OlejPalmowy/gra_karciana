#include "kartawidget.h"
#include "ui_kartawidget.h"
#include <QPixmap>
#include <QMouseEvent>

KartaWidget::KartaWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KartaWidget)
{
    ui->setupUi(this);
}

KartaWidget::~KartaWidget()
{
    delete ui;
}

void KartaWidget::ustawGrafike(const QString &sciezkaDoFoto)
{
    // wczytanie obrazka
    QPixmap foto(sciezkaDoFoto);

    // wrzucenie do labela
    ui->tloKarty->setPixmap(foto);

    // skalowanie zeby pasowalo
    ui->tloKarty->setScaledContents(true);
}

void KartaWidget::mousePressEvent(QMouseEvent *event)
{
    // wyslanie sygnalu po kliknieciu
    emit kartaKliknieta(this);
}

void KartaWidget::oznaczJakoWybrana(bool status)
{
    // pomin jesli juz ma ten status
    if (status == czyWybrana) return;

    int aktualnyX = this->x();
    int aktualnyY = this->y();

    if (status == true) {
        // podniesienie karty o 15px w gore
        this->move(aktualnyX, aktualnyY - 15);
        czyWybrana = true;
    } else {
        // opuszczenie karty
        this->move(aktualnyX, aktualnyY + 15);
        czyWybrana = false;
    }
}

void KartaWidget::ustawStatystyki(int obrazenia, int kosztPA, const QString &opis)
{
    // atak na czerwono
    ui->labelAtak->setText(QString::number(obrazenia));
    ui->labelAtak->setStyleSheet("color: #A30000; font-weight: bold; font-size: 14px;");

    // pa na niebiesko
    ui->labelPa->setText(QString::number(kosztPA));
    ui->labelPa->setStyleSheet("color: #0055B3; font-weight: bold; font-size: 14px;");

    // opis na dole
    ui->labelOpis->setText(opis);
    ui->labelOpis->setStyleSheet("color: white; font-size: 10px;");
    ui->labelOpis->setWordWrap(true); // zawijanie zeby nie odcielo tekstu
}