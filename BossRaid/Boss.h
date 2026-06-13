#ifndef BOSS_H
#define BOSS_H

#include "Postac.h"

/**
 * @brief Klasa reprezentująca bossa sterowanego przez system.
 * Dziedziczy po klasie bazowej Postac.
 */
class Boss : public Postac {
private:
    int faza;                       ///< Aktualna faza bossa (1 lub 2)
    bool wlasnieWszedlWFaze2 = false; ///< Flaga zabezpieczająca przejście

public:
    /**
     * @brief Konstruktor bossa.
     * @param n Nazwa bossa.
     * @param startHp Ilość początkowego życia.
     */
    Boss(std::string n, int startHp) : Postac(n, startHp) {
        faza = 1;
    }

    /**
     * @brief Implementacja wirtualnej metody - nadaje abstrakcji realny sens.
     * Resetuje cykl bossa.
     */
    void przygotujDoTury() override {
        wlasnieWszedlWFaze2 = false;
    }

    /** @brief Zwraca aktualną fazę bossa. @return Numer fazy. */
    int getFaza() const { return faza; }

    /**
     * @brief Oblicza i ewentualnie przełącza fazę bossa w zależności od zdrowia.
     * @return true tylko przy momencie przejścia między fazami.
     */
    bool aktualizujFaze() {
        if (faza == 1 && hp <= (maxHp / 2)) {
            faza = 2;
            dodajPancerz(15);
            wlasnieWszedlWFaze2 = true;
            return true;
        }
        return false;
    }

    /**
     * @brief Akcja ataku bossa z podziałem na fazy.
     * @param typAtaku Typ losowanego zachowania.
     * @return Ostateczna wartość zadanego damage'u do gracza.
     */
    int wykonajAtak(int typAtaku) {
        aktualizujFaze();

        int dmg = 0;
        if (faza == 1) {
            if (typAtaku == 1) dmg = 10;
            else if (typAtaku == 2) dmg = 18;
            else if (typAtaku == 3) {
                dodajPancerz(15);
                hp += 15;
                if (hp > maxHp) hp = maxHp;
                dmg = 0;
            }
        }
        else if (faza == 2) {
            if (typAtaku == 1) dmg = 18;
            else if (typAtaku == 2) dmg = 34;
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