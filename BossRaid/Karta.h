#ifndef KARTA_H
#define KARTA_H

#include <string>


class Karta {
private:
    std::string nazwa;
    int typJednostki;     // 1 = Rycerz, 2 = Łucznik, 3 = Mag
    int obrazeniaHP;      // Zwykłe obrażenia
    int obrazeniaPancerz; // Obrażenia niszczące tarczę
    int pancerzBonus;     // Ile pancerza daje graczowi
    int kosztPA;

public:
    // Konstruktor karty
    Karta(std::string n, int typ, int dmgHP, int dmgArmor, int armorBonus, int koszt) {
        nazwa = n;
        typJednostki = typ;
        obrazeniaHP = dmgHP;
        obrazeniaPancerz = dmgArmor;
        pancerzBonus = armorBonus;
        kosztPA = koszt;
    }
    // Gettery do odczytu statystyk karty (np. żeby wyświetlić je w Qt)
    std::string getNazwa() const { return nazwa; }
    int getTyp() const { return typJednostki; }
    int getObrazeniaHP() const { return obrazeniaHP; }
    int getObrazeniaPancerz() const { return obrazeniaPancerz; }
    int getPancerzBonus() const { return pancerzBonus; }
    int getKosztPA() const { return kosztPA; }
};

#endif // KARTA_H
