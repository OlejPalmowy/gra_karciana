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
    //kontruktor tutaj jest
    Postac(std::string n , int startHP) {
        nazwa = n;
        hp = startHP;
        maxHp = startHP;
        pancerz = 0;
    }
    virtual ~Postac()= default;

    //logika otrzymywania obrazen
    void otrzymajObrazenia(int dmg) {
        if (pancerz >0){
            if(pancerz>= dmg) {
                pancerz-=dmg;
                dmg=0;
            } else {
                dmg -= pancerz;
                pancerz = 0;
            }
        }
        hp -= dmg;
        if(hp<0)hp = 0;
    }
    // Funkcje pomocnicze (tzw. gettery i settery)
    int getHp() const { return hp; }
    int getMaxHp() const { return maxHp; }
    int getPancerz() const { return pancerz; }
    std::string getNazwa() const { return nazwa; }

    void dodajPancerz(int wartosc) { pancerz += wartosc; }
    void resetujPancerz() { pancerz = 0; }
};

#endif // POSTAC_H