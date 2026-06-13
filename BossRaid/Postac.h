#ifndef POSTAC_H
#define POSTAC_H

#include <string>

/**
 * @brief Abstrakcyjna klasa bazowa dla wszystkich uczestników rozgrywki.
 * Implementuje zarządzanie życiem, pancerzem oraz otrzymywaniem obrażeń.
 */
class Postac {
protected:
    std::string nazwa;  ///< Imię jednostki
    int hp;             ///< Bieżące punkty życia
    int maxHp;          ///< Maksymalne punkty życia bazowego
    int pancerz;        ///< Tarcza przyjmująca obrażenia w pierwszej kolejności

public:
    /**
     * @brief Inicjuje podstawowe właściwości bytu w grze.
     * @param n Nazwa.
     * @param startHP Ilość życia startowego/maksymalnego.
     */
    Postac(std::string n, int startHP) : nazwa(n), hp(startHP), maxHp(startHP), pancerz(0) {}

    /** @brief Wirtualny destruktor gwarantuje poprawne zwalnianie klas pochodnych. */
    virtual ~Postac() = default;

    /**
     * @brief ABSTRAKCJA: Czysto wirtualna funkcja wymuszająca implementację procedur początkowych tury.
     */
    virtual void przygotujDoTury() = 0;

    /**
     * @brief Logika otrzymywania obrażeń uwzględniająca mechaniczną absorpcję przez pancerz.
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

    /** @brief Zwraca obecne HP. @return Aktualna wartość życia. */
    int getHp() const { return hp; }
    /** @brief Zwraca maksymalne HP. @return Limit życia. */
    int getMaxHp() const { return maxHp; }
    /** @brief Zwraca Pancerz. @return Obecna tarcza. */
    int getPancerz() const { return pancerz; }
    /** @brief Zwraca Imię. @return Nazwa jednostki. */
    std::string getNazwa() const { return nazwa; }

    /**
     * @brief Dodaje punkty pancerza chroniącego postać.
     * @param wartosc Wartość, o którą zostanie powiększony pancerz.
     */
    void dodajPancerz(int wartosc) { pancerz += wartosc; }

    /** @brief Zeruje całkowicie licznik tarczy. */
    void resetujPancerz() { pancerz = 0; }
};

#endif // POSTAC_H