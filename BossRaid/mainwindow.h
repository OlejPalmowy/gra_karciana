#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include <vector>
#include <QSoundEffect> // Biblioteka do obsługi efektów dźwiękowych
#include "kartawidget.h"
#include "Gracz.h"
#include "Boss.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief Klasa głównego okna programu. Obsługuje zdarzenia i instancjuje grę.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor głównego obiektu.
     * @param parent Rodzic w hierarchii okienek Qt.
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /** @brief Destruktor widoku głównego. */
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;                                 ///< Pointer na layout Qt
    std::unique_ptr<Gracz> aktualnyGracz;               ///< Bezpieczny pointer (unique_ptr) na Gracza
    std::unique_ptr<Boss> bossRaidu;                    ///< Bezpieczny pointer (unique_ptr) na Bossa
    std::vector<KartaWidget*> widgetyWRece;             ///< Kontener na pointery widgetów z UI

    // Głośniki do efektów dźwiękowych gry
    QSoundEffect *sfxMiecz;                             ///< Dźwięk dla karty Rycerza
    QSoundEffect *sfxStrzala;                           ///< Dźwięk dla karty Łucznika
    QSoundEffect *sfxMagia;                             ///< Dźwięk dla karty Maga
    QSoundEffect *sfxDriada;                            ///< DODANE: Dźwięk dla karty Driady
    QSoundEffect *sfxWygrana;                           ///< Muzyka po pokonaniu Bossa
    QSoundEffect *sfxPrzegrana;                         ///< Dźwięk po śmierci Gracza

    /** @brief Losuje, tworzy za pomocą polimorfizmu instancję Karty i renderuje do UI. */
    void dodajLosowaKarte();

    /** @brief Odświeża warstwę tekstową interfejsu (HP, PA, Pancerz). */
    void aktualizujInterfejs();

    /** @brief Pokazuje komunikaty dla bohatera. */
    void dodajKomunikatGracza(const QString &tekst);

    /** @brief Pokazuje teksty bossa z eventów. */
    void dodajKomunikatBossa(const QString &tekst);

    /** @brief Detektor stanów End-Game. */
    void sprawdzKoniecGry(int ostatnieObrazenia = 0);

private slots:
    /** @brief Reakcja na klik w pojedynczą kartę. */
    void onKartaKliknieta(KartaWidget* kliknietaKarta);

    /** @brief Silnik zagrywania sekwencji wybranych kart. */
    void zagrajWybranaKarte();

    /** @brief Kończy turę gracza i przerzuca akcję na ataki Bossa. */
    void zakonczTure();

    /** @brief Moduł przetasowania i wylosowania 4 nowych kart. */
    void przerolujKarty();
};

#endif // MAINWINDOW_H