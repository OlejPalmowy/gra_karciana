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
     * @param event Wskaźnik na obiekt zdarzenia.
     */
    void mousePressEvent(QMouseEvent *event) override;

public:
    /**
     * @brief Konstruktor widżetu.
     * @param parent Wskaźnik na rodzica.
     */
    explicit KartaWidget(QWidget *parent = nullptr);

    /** @brief Destruktor zwalniający ui. */
    ~KartaWidget();

    /**
     * @brief Ładuje grafikę z zasobów.
     * @param sciezkaDoFoto Relatywna ścieżka qrc.
     */
    void ustawGrafike(const QString &sciezkaDoFoto);

    /**
     * @brief Zmienia stan wizualny (podniesienie).
     * @param status Stan bycia zaznaczonym.
     */
    void oznaczJakoWybrana(bool status);

    /**
     * @brief Getter sprawdzający status zaznaczenia (Enkapsulacja).
     * @return True jeśli podświetlona.
     */
    bool getCzyWybrana() const { return czyWybrana; }

    /**
     * @brief Renderuje bazowe statystyki na widget-cie UI.
     * @param obrazenia Obrażenia na kartę.
     * @param kosztPA Wartość kosztu PA.
     * @param opis Własny opis tekstu na labelu.
     */
    void ustawStatystyki(int obrazenia, int kosztPA, const QString &opis);

signals:
    /**
     * @brief Emituje sygnał natychmiastowo po detekcji zdarzenia `mousePress`.
     * @param kliknietaKarta Wskaźnik this.
     */
    void kartaKliknieta(KartaWidget* kliknietaKarta);

private:
    Ui::KartaWidget *ui;            ///< Interfejs UI.
    bool czyWybrana = false;        ///< Prywatna flaga określająca stan logiczny karty.
};

#endif // KARTAWIDGET_H