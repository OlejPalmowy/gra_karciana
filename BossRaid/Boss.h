#ifndef BOSS_H
#define BOSS_H

#include "Postac.h"

// Boss też dziedziczy po Postaci (ma HP, pancerz i funkcję otrzymywania obrażeń)
class Boss : public Postac {
private:
    int faza; // 1 = Normalny, 2 = Wkurzony

public:
    // Konstruktor Bossa
    Boss(std::string n, int startHp) : Postac(n, startHp) {
        faza = 1; // Zawsze zaczyna na luzie
    }

    int getFaza() const { return faza; }

    // Wewnętrzna logika sprawdzająca, czy Boss ma już dość
    void aktualizujFaze() {
        // Jeśli ma połowę HP lub mniej i jest w 1 fazie, wchodzi w 2 fazę
        if (faza == 1 && hp <= (maxHp / 2)) {
            faza = 2;
        }
    }

    // Funkcja do zadawania obrażeń graczowi - interfejs wywoła to co turę
    int wykonajAtak() {
        aktualizujFaze(); // Przed atakiem zawsze sprawdzamy, czy się nie wkurzył

        if (faza == 1) {
            return 15; // Zwykły, bazowy atak
        } else {
            return 30; // Atak furii w 2 fazie
        }
    }
};

#endif // BOSS_H
