#ifndef POSTAC_H
#define POSTAC_H

#include <string>

class Postac {
protected:
    std::string nazwa;
    int hp;
    int maxHp;
    int pancerz;

public:
    Postac(std::string n, int startHP) {
        nazwa = n;
        hp = startHP;
        maxHp = startHP;
        pancerz = 0;
    }
    virtual ~Postac() = default;

    /**
     * @brief ABSTRAKCJA: Czysto wirtualna funkcja wymuszająca implementację w klasach pochodnych.
     */
    virtual void przygotujDoTury() = 0;

    /**
     * @brief Logika otrzymywania obrażeń z uwzględnieniem pancerza.
     * @param dmg Ilość obrażeń wejściowych.
     */
    void otrzymajObrazenia(int dmg) {
        if (pancerz > 0) {
            if (pancerz >= dmg) {
                pancerz -= dmg;
                dmg = 0;
            } else {
                dmg -= pancerz;
                pancerz = 0;
            }
        }
        hp -= dmg;
        if (hp < 0) hp = 0;
    }

    /**
     * @brief Zwraca obecne punkty życia.
     * @return Obecne HP.
     */
    int getHp() const { return hp; }
    int getMaxHp() const { return maxHp; }
    int getPancerz() const { return pancerz; }
    std::string getNazwa() const { return nazwa; }

    /**
     * @brief Dodaje punkty pancerza postaci.
     * @param wartosc Ilość pancerza do dodania.
     */
    void dodajPancerz(int wartosc) { pancerz += wartosc; }
    void resetujPancerz() { pancerz = 0; }
};

#endif // POSTAC_H