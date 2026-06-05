#ifndef KARTAWIDGET_H
#define KARTAWIDGET_H

#include <QWidget>

namespace Ui {
class KartaWidget;
}

class KartaWidget : public QWidget
{
    Q_OBJECT

protected:
    void mousePressEvent(QMouseEvent *event) override;

public:
    explicit KartaWidget(QWidget *parent = nullptr);
    ~KartaWidget();
    void ustawGrafike(const QString &sciezkaDoFoto);
    void oznaczJakoWybrana(bool status);
    bool czyWybrana = false;

signals:
    void kartaKliknieta(KartaWidget* kliknietaKarta);

private:
    Ui::KartaWidget *ui;
};

#endif // KARTAWIDGET_H
