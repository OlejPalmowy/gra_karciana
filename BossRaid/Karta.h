#ifndef KARTA_H
#define KARTA_H

#include <string>

/**
 * @brief Klasa reprezentująca pojedynczą kartę w grze.
 */
class Karta {
private:
    std::string nazwa;
    int typJednostki;     // 1 = Rycerz, 2 = Łucznik, 3 = Mag, 4 = Uzdrowicielka
    int obrazeniaHP;      // Zwykłe obrażenia
    int obrazeniaPancerz; // Obrażenia niszczące tarczę
    int pancerzBonus;     // Ile pancerza daje graczowi
    int kosztPA;

public:
    /**
     * @brief Konstruktor obiektu Karta.
     * @param n Nazwa karty.
     * @param typ Typ jednostki (1-Rycerz, 2-Łucznik, 3-Mag, 4-Uzdrowicielka).
     * @param dmgHP Obrażenia zadawane bezpośrednio w punkty życia.
     * @param dmgArmor Obrażenia zadawane w pancerz.
     * @param armorBonus Bonus do pancerza dla gracza.
     * @param koszt Koszt zagrania karty w Punktach Akcji (PA).
     */
    Karta(std::string n, int typ, int dmgHP, int dmgArmor, int armorBonus, int koszt) {
        nazwa = n;
        typJednostki = typ;
        obrazeniaHP = dmgHP;
        obrazeniaPancerz = dmgArmor;
        pancerzBonus = armorBonus;
        kosztPA = koszt;
    }

    /**
     * @brief Zwraca nazwę karty.
     * @return Ciąg znaków z nazwą.
     */
    std::string getNazwa() const { return nazwa; }

    /**
     * @brief Zwraca typ jednostki.
     * @return Liczba całkowita reprezentująca typ.
     */
    int getTyp() const { return typJednostki; }

    /**
     * @brief Zwraca obrażenia w HP.
     * @return Wartość obrażeń.
     */
    int getObrazeniaHP() const { return obrazeniaHP; }

    /**
     * @brief Zwraca obrażenia w pancerz.
     * @return Wartość obrażeń w pancerz.
     */
    int getObrazeniaPancerz() const { return obrazeniaPancerz; }

    /**
     * @brief Zwraca bonusowy pancerz dla gracza.
     * @return Wartość bonusu.
     */
    int getPancerzBonus() const { return pancerzBonus; }

    /**
     * @brief Zwraca koszt zagrania karty w PA.
     * @return Koszt w punktach akcji.
     */
    int getKosztPA() const { return kosztPA; }
};

#endif // KARTA_H