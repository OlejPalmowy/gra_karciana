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
    QPixmap foto(sciezkaDoFoto);
    ui->tloKarty->setPixmap(foto);
    ui->tloKarty->setScaledContents(true);
}

void KartaWidget::mousePressEvent(QMouseEvent *event)
{
    emit kartaKliknieta(this);
}

void KartaWidget::oznaczJakoWybrana(bool status)
{
    if (status == czyWybrana) return;
    int aktualnyX = this->x();
    int aktualnyY = this->y();

    if (status == true) {
        this->move(aktualnyX, aktualnyY - 15);
        czyWybrana = true;
    } else {
        this->move(aktualnyX, aktualnyY + 15);
        czyWybrana = false;
    }
}

void KartaWidget::ustawStatystyki(int obrazenia, int kosztPA, const QString &opis)
{
    ui->labelAtak->setText(QString::number(obrazenia));
    ui->labelAtak->setStyleSheet("color: #A30000; font-weight: bold; font-size: 14px;");

    ui->labelPa->setText(QString::number(kosztPA));
    ui->labelPa->setStyleSheet("color: #0055B3; font-weight: bold; font-size: 14px;");

    ui->labelOpis->setText(opis);
    ui->labelOpis->setStyleSheet("color: white; font-size: 10px;");
    ui->labelOpis->setWordWrap(true);
}