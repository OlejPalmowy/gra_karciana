#include "kartawidget.h"
#include "ui_kartawidget.h"
#include <QPixmap> // To musimy dodać na górze, żeby obsłużyć obrazki
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

// TO DOPISUJEMY: Ta funkcja robi całą robotę
void KartaWidget::ustawGrafike(const QString &sciezkaDoFoto)
{
    // Tworzymy obiekt grafiki na podstawie podanej ścieżki
    QPixmap foto(sciezkaDoFoto);

    // Wrzucamy grafikę do Twojego Labela (upewnij się, że tak go nazwałeś w objectName!)
    ui->tloKarty->setPixmap(foto);

    // Opcjonalnie: wymuszamy, żeby obrazek idealnie dopasował się do wymiarów Labela
    ui->tloKarty->setScaledContents(true);
}

// Ta funkcja odpala się automatycznie, gdy klikniesz w kartę
void KartaWidget::mousePressEvent(QMouseEvent *event)
{
    // Karta wysyła sygnał w świat i przekazuje wskaźnik na samą siebie
    emit kartaKliknieta(this);
}

void KartaWidget::oznaczJakoWybrana(bool status)
{
    // Jeśli próbujemy ustawić stan, który już jest aktywny – przerywamy
    if (status == czyWybrana) return;

    int aktualnyX = this->x();
    int aktualnyY = this->y();

    if (status == true) {
        // Przesuwamy kartę o 15 pikseli w górę
        this->move(aktualnyX, aktualnyY - 15);
        czyWybrana = true;
    } else {
        // Zwracamy kartę na pierwotną pozycję (w dół)
        this->move(aktualnyX, aktualnyY + 15);
        czyWybrana = false;
    }
}

void KartaWidget::ustawStatystyki(int obrazenia, int kosztPA)
{
    // Zamieniamy liczby (int) na tekst (QString), żeby Label mógł je wyświetlić
    // Upewnij się, że nazwy "labelAtak" i "labelHP" zgadzają się z tymi w Qt Designerze!
    ui->labelAtak->setText(QString::number(obrazenia));
    ui->labelPa->setText(QString::number(kosztPA));
}