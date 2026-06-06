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
    // Zmieniamy funkcję, by przyjmowała wylosowany "typ" ataku z zewnątrz
    int wykonajAtak(int typAtaku) {
        aktualizujFaze();

        int dmg = 0;

        // 1 = Zwykły cios, 2 = Silny cios, 3 = Obrona
        if (typAtaku == 1) {
            dmg = 15;
        }
        else if (typAtaku == 2) {
            dmg = 25;
        }
        else if (typAtaku == 3) {
            dodajPancerz(15);
            dmg = 0; // Nie zadaje obrażeń, bo się broni
        }

        // Jeśli ma mało HP (Faza 2), bije podwójnie!
        if (faza == 2) {
            dmg *= 2;
        }

        return dmg;
    }
};

#endif // BOSS_H
