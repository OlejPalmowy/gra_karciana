#ifndef KARTA_H
#define KARTA_H

#include <string>
#include <QString>
#include "Boss.h"
#include "Gracz.h"

/**
 * @brief Abstrakcyjna klasa bazowa reprezentująca kartę w grze.
 * Wykorzystuje polimorfizm do implementacji unikalnych zachowań kart.
 */
class Karta {
protected:
    std::string nazwa;      ///< Nazwa karty
    int typJednostki;       ///< 1 = Rycerz, 2 = Łucznik, 3 = Mag, 4 = Uzdrowicielka
    int obrazeniaHP;        ///< Obrażenia zadawane bezpośrednio w punkty życia
    int obrazeniaPancerz;   ///< Obrażenia zadawane w pancerz
    int pancerzBonus;       ///< Ile pancerza daje graczowi
    int kosztPA;            ///< Koszt zagrania w Punktach Akcji

public:
    /**
     * @brief Konstruktor bazowy dla wszystkich kart.
     * @param n Nazwa karty.
     * @param typ Typ jednostki.
     * @param dmgHP Obrażenia HP.
     * @param dmgArmor Obrażenia pancerza.
     * @param armorBonus Bonus do pancerza gracza.
     * @param koszt Koszt PA.
     */
    Karta(std::string n, int typ, int dmgHP, int dmgArmor, int armorBonus, int koszt)
        : nazwa(n), typJednostki(typ), obrazeniaHP(dmgHP),
        obrazeniaPancerz(dmgArmor), pancerzBonus(armorBonus), kosztPA(koszt) {}

    /** @brief Wirtualny destruktor dla poprawnego niszczenia klas pochodnych. */
    virtual ~Karta() = default;

    /**
     * @brief Czysto wirtualna metoda do zagrania karty (POLIMORFIZM).
     * @param boss Wskaźnik na bossa.
     * @param gracz Wskaźnik na gracza.
     * @return Opcjonalny komunikat do wyświetlenia w interfejsie logów (lub puste, jeśli brak).
     */
    virtual QString zagraj(Boss* boss, Gracz* gracz) = 0;

    /** @brief Zwraca nazwę karty. @return Ciąg znaków z nazwą. */
    std::string getNazwa() const { return nazwa; }
    /** @brief Zwraca typ jednostki. @return Typ jednostki (1-4). */
    int getTyp() const { return typJednostki; }
    /** @brief Zwraca obrażenia HP. @return Wartość obrażeń. */
    int getObrazeniaHP() const { return obrazeniaHP; }
    /** @brief Zwraca obrażenia pancerza. @return Wartość obrażeń w pancerz. */
    int getObrazeniaPancerz() const { return obrazeniaPancerz; }
    /** @brief Zwraca bonus do pancerza. @return Wartość pancerza dodawanego graczowi. */
    int getPancerzBonus() const { return pancerzBonus; }
    /** @brief Zwraca koszt zagrania. @return Koszt w punktach akcji (PA). */
    int getKosztPA() const { return kosztPA; }
};

/**
 * @brief Klasa pochodna reprezentująca kartę Rycerza.
 */
class KartaRycerz : public Karta {
public:
    KartaRycerz() : Karta("Rycerz", 1, 3, 9, 5, 3) {}
    QString zagraj(Boss* boss, Gracz* gracz) override {
        int pancerzBossa = boss->getPancerz();
        if (pancerzBossa > 0) {
            if (pancerzBossa >= getObrazeniaPancerz()) {
                boss->otrzymajObrazenia(getObrazeniaPancerz());
            } else {
                boss->otrzymajObrazenia(pancerzBossa);
            }
        } else {
            boss->otrzymajObrazenia(getObrazeniaHP());
        }
        gracz->dodajPancerz(getPancerzBonus());
        return "";
    }
};

/**
 * @brief Klasa pochodna reprezentująca kartę Łucznika.
 */
class KartaLucznik : public Karta {
public:
    KartaLucznik() : Karta("Łucznik", 2, 4, 3, 0, 1) {}
    QString zagraj(Boss* boss, Gracz* gracz) override {
        int pancerzBossa = boss->getPancerz();
        if (pancerzBossa > 0) {
            if (pancerzBossa >= getObrazeniaPancerz()) {
                boss->otrzymajObrazenia(getObrazeniaPancerz());
            } else {
                boss->otrzymajObrazenia(pancerzBossa);
            }
        } else {
            boss->otrzymajObrazenia(getObrazeniaHP());
        }
        return "";
    }
};

/**
 * @brief Klasa pochodna reprezentująca kartę Maga.
 */
class KartaMag : public Karta {
public:
    KartaMag() : Karta("Mag", 3, 15, 3, 0, 3) {}
    QString zagraj(Boss* boss, Gracz* gracz) override {
        int pancerzBossa = boss->getPancerz();
        boss->resetujPancerz();
        boss->otrzymajObrazenia(getObrazeniaHP());
        boss->dodajPancerz(pancerzBossa);
        return "";
    }
};

/**
 * @brief Klasa pochodna reprezentująca kartę Driady (Uzdrowicielki).
 */
class KartaDriada : public Karta {
public:
    KartaDriada() : Karta("Driada", 4, 0, 0, 0, 0) {}
    QString zagraj(Boss* boss, Gracz* gracz) override {
        gracz->ulecz(10);
        return "Uzdrowicielka leczy Twoje rany! (+10 HP)";
    }
};

#endif // KARTA_H