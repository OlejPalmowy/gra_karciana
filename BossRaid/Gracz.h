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
    int bonusPA = 0; // Dodano zmienną do bonusu z Łucznika

public:
    Gracz(std::string n, int startHp, int startPA) : Postac(n, startHp) {
        maxPunktyAkcji = startPA;
        punktyAkcji = startPA;
    }

    void usunKarteZReki(int indeks) {
        if (indeks >= 0 && indeks < reka.size()) {
            reka.erase(reka.begin() + indeks);
        }
    }

    // Funkcja do leczenia
    void ulecz(int wartosc) {
        hp += wartosc;
        if (hp > maxHp) {
            hp = maxHp; // Zabezpieczenie, żeby nie przekroczyć Max HP
        }
    }

    // Funkcja do Rerolla
    void wyczyscReke() {
        reka.clear();
    }

    int getPA() const { return punktyAkcji; }
    std::vector<Karta> getReka() const { return reka; }

    void dodajBonusPA(int wartosc) {
        bonusPA += wartosc;
    }

    void odnowPA() {
        // Dodajemy stałą wartość (np. 3) + to, co wypracował Łucznik
        punktyAkcji += (9 + bonusPA);
        bonusPA = 0; // Zerujemy bonus
    }

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