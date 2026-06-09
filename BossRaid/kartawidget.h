#ifndef KARTAWIDGET_H
#define KARTAWIDGET_H

#include <QWidget>

namespace Ui {
class KartaWidget;
}

/**
 * @brief Klasa reprezentująca graficzny interfejs pojedynczej karty na ekranie.
 */
class KartaWidget : public QWidget
{
    Q_OBJECT

protected:
    /**
     * @brief Obsługa zdarzenia kliknięcia myszą w kartę.
     * @param event Wskaźnik na obiekt zdarzenia systemowego.
     */
    void mousePressEvent(QMouseEvent *event) override;

public:
    /**
     * @brief Konstruktor widżetu karty.
     * @param parent Wskaźnik na widżet nadrzędny (domyślnie nullptr).
     */
    explicit KartaWidget(QWidget *parent = nullptr);

    /**
     * @brief Destruktor widżetu karty.
     */
    ~KartaWidget();

    /**
     * @brief Ustawia grafikę tła dla karty na podstawie pliku.
     * @param sciezkaDoFoto Ścieżka dostępu do pliku z grafiką.
     */
    void ustawGrafike(const QString &sciezkaDoFoto);

    /**
     * @brief Zmienia wizualny stan karty (np. podnosi ją do góry, gdy jest wybrana).
     * @param status True jeśli karta ma zostać oznaczona jako wybrana, false w przeciwnym razie.
     */
    void oznaczJakoWybrana(bool status);

    bool czyWybrana = false; ///< Flaga określająca, czy gracz zaznaczył tę kartę do zagrania.

    /**
     * @brief Ustawia statystyki wyświetlane bezpośrednio na karcie.
     * @param obrazenia Wartość zadawanych obrażeń.
     * @param kosztPA Koszt zagrania w Punktach Akcji.
     * @param opis Tekstowy opis specjalnych właściwości karty.
     */
    void ustawStatystyki(int obrazenia, int kosztPA, const QString &opis);

signals:
    /**
     * @brief Sygnał emitowany natychmiast po kliknięciu w kartę przez gracza.
     * @param kliknietaKarta Wskaźnik na kliknięty obiekt KartaWidget.
     */
    void kartaKliknieta(KartaWidget* kliknietaKarta);

private:
    Ui::KartaWidget *ui;
};

#endif // KARTAWIDGET_H