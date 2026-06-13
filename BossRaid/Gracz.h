#ifndef GRACZ_H
#define GRACZ_H

#include "Postac.h"
#include <vector>
#include <memory>

class Karta; // Forward declaration dla uniknięcia pętli nagłówków

/**
 * @brief Klasa reprezentująca głównego bohatera (Gracza).
 * Dziedziczy po klasie bazowej Postac.
 */
class Gracz : public Postac {
private:
    int punktyAkcji;            ///< Obecne punkty akcji (PA) gracza
    int maxPunktyAkcji;         ///< Maksymalne punkty akcji
    std::vector<std::shared_ptr<Karta>> reka; ///< Zbiór kart na ręce zarządzany przez inteligentne wskaźniki
    int bonusPA = 0;            ///< Premia do PA w następnej turze

public:
    /**
     * @brief Konstruktor obiektu gracza.
     * @param n Nazwa gracza.
     * @param startHp Początkowe (i maksymalne) zdrowie.
     * @param startPA Początkowe punkty akcji.
     */
    Gracz(std::string n, int startHp, int startPA) : Postac(n, startHp) {
        maxPunktyAkcji = startPA;
        punktyAkcji = startPA;
    }

    /**
     * @brief Implementacja wirtualnej metody - nadaje abstrakcji realny sens.
     * Gracz przygotowując się do tury, odnawia swoje punkty akcji.
     */
    void przygotujDoTury() override {
        punktyAkcji += (9 + bonusPA);
        bonusPA = 0;
    }

    /**
     * @brief Usuwa kartę z ręki na podstawie indeksu.
     * @param indeks Indeks karty do usunięcia.
     */
    void usunKarteZReki(int indeks) {
        if (indeks >= 0 && indeks < reka.size()) {
            reka.erase(reka.begin() + indeks);
        }
    }

    /**
     * @brief Leczy gracza, pamiętając o limitach maksymalnego HP.
     * @param wartosc Ilość punktów do wyleczenia.
     */
    void ulecz(int wartosc) {
        hp += wartosc;
        if (hp > maxHp) {
            hp = maxHp;
        }
    }

    /** @brief Czyści całkowicie rękę gracza z kart. */
    void wyczyscReke() {
        reka.clear();
    }

    /** @brief Zwraca ilość punktów akcji. @return Obecne punkty PA. */
    int getPA() const { return punktyAkcji; }

    /** @brief Zwraca wektor kart w ręce. @return Ręka gracza. */
    std::vector<std::shared_ptr<Karta>> getReka() const { return reka; }

    /**
     * @brief Dodaje wartość do bonusu punktów akcji.
     * @param wartosc Wartość bonusu.
     */
    void dodajBonusPA(int wartosc) {
        bonusPA += wartosc;
    }

    /**
     * @brief Zmniejsza ilość PA o określony koszt (jeśli to możliwe).
     * @param koszt Koszt zagrania.
     * @return true, jeśli odjęto PA, false jeśli gracz ma ich za mało.
     */
    bool zuzyjPA(int koszt) {
        if (punktyAkcji >= koszt) {
            punktyAkcji -= koszt;
            return true;
        }
        return false;
    }

    /**
     * @brief Dodaje nową kartę do ręki.
     * @param k Inteligentny wskaźnik na dodawaną kartę.
     */
    void dodajKarteDoReki(std::shared_ptr<Karta> k) {
        reka.push_back(k);
    }
};
#endif // GRACZ_H