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

        if (wlasnieWszedlWFaze2) {
            dodajPancerz(30);
            wlasnieWszedlWFaze2 = false;
        }

        if (typAtaku == 1) {
            dmg = 15;
        }
        else if (typAtaku == 2) {
            dmg = 25;
        }
        else if (typAtaku == 3) {
            dodajPancerz(15);
            dmg = 0;
        }

        if (faza == 2) {
            dmg *= 2;
        }

        return dmg;
    }
};
#endif // BOSS_H