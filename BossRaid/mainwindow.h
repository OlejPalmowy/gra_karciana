#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "kartawidget.h"
#include <QMainWindow>
#include "Gracz.h"
#include "Boss.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief Klasa reprezentująca główne okno aplikacji oraz zarządzająca pętlą logiki gry.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor głównego okna gry. Inicjalizuje interfejs i logikę postaci.
     * @param parent Wskaźnik na nadrzędny widżet (domyślnie nullptr).
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destruktor okna. Zwalnia zasoby interfejsu użytkownika.
     */
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    Gracz *aktualnyGracz; ///< Wskaźnik na obiekt reprezentujący gracza.
    Boss *bossRaidu;      ///< Wskaźnik na obiekt reprezentujący przeciwnika (bossa).
    std::vector<KartaWidget*> widgetyWRece; ///< Wektor przechowujący wizualne reprezentacje kart na ręce gracza.

    /**
     * @brief Losuje nową kartę i dodaje ją zarówno do logiki gracza, jak i do interfejsu graficznego.
     */
    void dodajLosowaKarte();

    /**
     * @brief Odświeża wszystkie wartości liczbowe (HP, AP, Pancerz) na interfejsie użytkownika.
     */
    void aktualizujInterfejs();

    /**
     * @brief Wyświetla komunikat tekstowy w logu wydarzeń gracza (np. informacje o użyciu karty).
     * @param tekst Treść komunikatu do wyświetlenia.
     */
    void dodajKomunikatGracza(const QString &tekst);

    /**
     * @brief Wyświetla tekst wypowiadany lub akcję wykonywaną przez bossa.
     * @param tekst Treść komunikatu bossa.
     */
    void dodajKomunikatBossa(const QString &tekst);

    /**
     * @brief Sprawdza warunki wygranej lub przegranej po każdej akcji i ewentualnie kończy grę.
     * @param ostatnieObrazenia Wartość obrażeń, które doprowadziły do ewentualnej śmierci postaci (domyślnie 0).
     */
    void sprawdzKoniecGry(int ostatnieObrazenia = 0);

private slots:
    /**
     * @brief Slot reagujący na kliknięcie w konkretną kartę na ekranie (zaznaczenie/odznaczenie).
     * @param kliknietaKarta Wskaźnik na kliknięty widżet karty.
     */
    void onKartaKliknieta(KartaWidget* kliknietaKarta);

    /**
     * @brief Slot wywoływany przyciskiem "Zagraj". Przetwarza logikę wszystkich zaznaczonych kart.
     */
    void zagrajWybranaKarte();

    /**
     * @brief Slot wywoływany przyciskiem końca tury. Inicjuje atak bossa i nową turę gracza.
     */
    void zakonczTure();

    /**
     * @brief Slot wymieniający wszystkie karty w ręce gracza za określoną ilość Punktów Akcji.
     */
    void przerolujKarty();
};

#endif // MAINWINDOW_H