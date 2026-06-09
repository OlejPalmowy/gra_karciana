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

    /**
     * @brief POLIMORFIZM: Implementacja funkcji wirtualnej z klasy Postac.
     */
    void przygotujDoTury() override {
        // Pusta implementacja spełniająca wymóg polimorfizmu
    }

    int getFaza() const { return faza; }

    /**
     * @brief Sprawdza i aktualizuje fazę bossa w zależności od HP.
     * @return true TYLKO w momencie przejścia z fazy 1 do 2, w przeciwnym razie false.
     */
    bool aktualizujFaze() {
        if (faza == 1 && hp <= (maxHp / 2)) {
            faza = 2;
            dodajPancerz(15);
            return true;
        }
        return false;
    }

    /**
     * @brief Wykonuje atak bossa o określonym typie, zależnie od aktualnej fazy.
     * @param typAtaku Numer typu ataku (1 - zwykły, 2 - silny, 3 - leczenie).
     * @return Ilość zadanych obrażeń.
     */
    int wykonajAtak(int typAtaku) {
        aktualizujFaze();

        int dmg = 0;

        if (faza == 1) {
            if (typAtaku == 1) {
                dmg = 10;
            }
            else if (typAtaku == 2) {
                dmg = 18;
            }
            else if (typAtaku == 3) {
                dodajPancerz(15);
                hp += 15;
                if (hp > maxHp) hp = maxHp;
                dmg = 0;
            }
        }
        else if (faza == 2) {
            if (typAtaku == 1) {
                dmg = 18;
            }
            else if (typAtaku == 2) {
                dmg = 34;
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