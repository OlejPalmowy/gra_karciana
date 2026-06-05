#ifndef GRACZ_H
#define GRACZ_H

#include "Postac.h"
#include "Karta.h"
#include <vector>

// Ten dwukropek oznacza: Gracz "dziedziczy" po klasie Postac (ma jej HP i pancerz z automatu)
class Gracz : public Postac {
private:
    int punktyAkcji;
    int maxPunktyAkcji;

    // Wektor to taka "gumowa" tablica. Idealna na karty, bo ręka gracza może rosnąć i maleć.
    std::vector<Karta> reka;

public:
    // Konstruktor Gracza.
    // Magia po dwukropku ( : Postac(...) ) to wysłanie nazwy i HP do "fabryki" klasy bazowej
    Gracz(std::string n, int startHp, int startPA) : Postac(n, startHp) {
        maxPunktyAkcji = startPA;
        punktyAkcji = startPA;
    }
    // Gettery
    int getPA() const { return punktyAkcji; }
    std::vector<Karta> getReka() const { return reka; }

    // Zarządzanie Punktami Akcji
    void resetujPA() {
        punktyAkcji = maxPunktyAkcji;
    }

    bool zuzyjPA(int koszt) {
        if (punktyAkcji >= koszt) {
            punktyAkcji -= koszt;
            return true; // Udało się, gracz miał wystarczająco PA
        }
        return false; // Brakuje PA, akcja odrzucona!
    }

    // Zarządzanie kartami
    void dodajKarteDoReki(Karta k) {
        reka.push_back(k); // push_back po prostu wciska nową kartę na sam koniec ręki
    }
};

#endif // GRACZ_H
