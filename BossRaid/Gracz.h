#ifndef GRACZ_H
#define GRACZ_H

#include "Postac.h"
#include "Karta.h"
#include <vector>

class Gracz : public Postac {
private:
    int punktyAkcji;
    int maxPunktyAkcji;
    std::vector<Karta> reka;
    int bonusPA = 0;

public:
    Gracz(std::string n, int startHp, int startPA) : Postac(n, startHp) {
        maxPunktyAkcji = startPA;
        punktyAkcji = startPA;
    }

    /**
     * @brief POLIMORFIZM: Implementacja funkcji wirtualnej z klasy Postac.
     */
    void przygotujDoTury() override {
        // Pusta implementacja spełniająca wymóg polimorfizmu
    }

    void usunKarteZReki(int indeks) {
        if (indeks >= 0 && indeks < reka.size()) {
            reka.erase(reka.begin() + indeks);
        }
    }

    /**
     * @brief Leczy gracza, nie przekraczając maksymalnego HP.
     * @param wartosc Ilość punktów życia do przywrócenia.
     */
    void ulecz(int wartosc) {
        hp += wartosc;
        if (hp > maxHp) {
            hp = maxHp;
        }
    }

    void wyczyscReke() {
        reka.clear();
    }

    int getPA() const { return punktyAkcji; }
    std::vector<Karta> getReka() const { return reka; }

    void dodajBonusPA(int wartosc) {
        bonusPA += wartosc;
    }

    void odnowPA() {
        punktyAkcji += (9 + bonusPA);
        bonusPA = 0;
    }

    /**
     * @brief Próbuje zużyć punkty akcji gracza.
     * @param koszt Ilość punktów akcji potrzebna do wykonania czynności.
     * @return true jeśli gracz miał wystarczająco PA, w przeciwnym razie false.
     */
    bool zuzyjPA(int koszt) {
        if (punktyAkcji >= koszt) {
            punktyAkcji -= koszt;
            return true;
        }
        return false;
    }

    void dodajKarteDoReki(Karta k) {
        reka.push_back(k);
    }
};
#endif // GRACZ_H