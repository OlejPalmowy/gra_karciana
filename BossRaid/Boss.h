#ifndef BOSS_H
#define BOSS_H

#include "Postac.h"

class Boss : public Postac {
private:
    int faza;
    bool wlasnieWszedlWFaze2 = false;

public:
    Boss(std::string n, int startHp) : Postac(n, startHp) {
        faza = 1;
    }

    int getFaza() const { return faza; }

    // ZMIANA: Zwraca true TYLKO w momencie przejścia z fazy 1 do 2
    bool aktualizujFaze() {
        if (faza == 1 && hp <= (maxHp / 2)) {
            faza = 2;
            dodajPancerz(15); // Dostaje skromniejsze 15 pancerza na start furii
            return true;
        }
        return false;
    }

    int wykonajAtak(int typAtaku) {
        aktualizujFaze();

        int dmg = 0;

        // BAZOWE WARTOŚCI DLA FAZY 1
        if (faza == 1) {
            if (typAtaku == 1) {
                dmg = 10; // Zwykły cios w 1. fazie
            }
            else if (typAtaku == 2) {
                dmg = 18; // Silny cios w 1. fazie (zamiast 25!)
            }
            else if (typAtaku == 3) {
                dodajPancerz(15);
                    hp += 15;
                    if (hp > maxHp) hp = maxHp;
                    dmg = 0;
            }
        }
        // WZMOCNIONE WARTOŚCI DLA FAZY 2 (FURIA)
        else if (faza == 2) {
            if (typAtaku == 1) {
                dmg = 18; // Wściekły zwykły cios
            }
            else if (typAtaku == 2) {
                dmg = 34; // Wściekły potężny cios (Twój idealny balans)
            }
            else if (typAtaku == 3) {
                dodajPancerz(15);
                hp += 15;
                if (hp > maxHp) hp = maxHp;
                dmg = 0;
            }
        }

        return dmg;
    }
};
#endif // BOSS_H