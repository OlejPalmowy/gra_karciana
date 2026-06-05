#ifndef KARTA_H
#define KARTA_H

#include <string>


class Karta {
private:
    std::string nazwa;
    int typJednostki;// 1 = Miecznik, 2 = Lucznik, 3 = Mag
    int obrazenia;
    int pancerzBonus;
    int kosztPA;      // Ile Punktów Akcji kosztuje zagranie

public:
    // Konstruktor karty
    Karta(std::string n, int typ, int dmg, int armor, int koszt) {
        nazwa = n;
        typJednostki = typ;
        obrazenia = dmg;
        pancerzBonus = armor;
        kosztPA = koszt;
    }
    // Gettery do odczytu statystyk karty (np. żeby wyświetlić je w Qt)
    std::string getNazwa() const { return nazwa; }
    int getTyp() const { return typJednostki; }
    int getObrazenia() const { return obrazenia; }
    int getPancerzBonus() const { return pancerzBonus; }
    int getKosztPA() const { return kosztPA; }
};

#endif // KARTA_H
