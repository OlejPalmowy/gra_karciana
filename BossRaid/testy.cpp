#include <QtTest>
#include "Postac.h"
#include "Gracz.h"
#include "Boss.h"

class GraTest : public QObject
{
    Q_OBJECT

private slots:
    // Test 1: Sprawdza czy pancerz poprawnie pochłania obrażenia
    void testPancerzPochlaniaObrazenia() {
        Postac p("Testowy Wojownik", 100);
        p.dodajPancerz(20);
        p.otrzymajObrazenia(15);

        QCOMPARE(p.getPancerz(), 5); // Zostało 5 pancerza
        QCOMPARE(p.getHp(), 100);     // Życie nienaruszone
    }

    // Test 2: Sprawdza przebicie pancerza i spadek HP
    void testPrzebiciePancerzaDoHP() {
        Postac p("Testowy Mag", 100);
        p.dodajPancerz(10);
        p.otrzymajObrazenia(25);

        QCOMPARE(p.getPancerz(), 0);  // Pancerz pękł
        QCOMPARE(p.getHp(), 85);      // 15 dmg weszło w HP
    }

    // Test 3: Logika zarządzania Punktami Akcji (PA) gracza
    void testPunktyAkcjiGracza() {
        Gracz g("Bohater", 30, 10);

        bool udaneZuzycie = g.zuzyjPA(4);
        QVERIFY(udaneZuzycie == true);
        QCOMPARE(g.getPA(), 6);

        bool zaDrogieZuzycie = g.zuzyjPA(8); // Koszt wyższy niż obecne 6 PA
        QVERIFY(zaDrogieZuzycie == false);
        QCOMPARE(g.getPA(), 6);             // Punkty nie zostały pobrane
    }

    // Test 4: Sprawdzenie automatycznego wejścia Bossa w 2. Fazę (Furię)
    void testFazaFuriiBossa() {
        Boss b("Mroczny Test", 100);
        QCOMPARE(b.getFaza(), 1);

        b.otrzymajObrazenia(60); // HP spada do 40 (czyli poniżej 50%)
        b.wykonajAtak(1);        // Wywołanie ataku odpala aktualizujFaze()

        QCOMPARE(b.getFaza(), 2); // Boss powinien być wściekły
    }

    // Test 5: Sprawdzenie, czy leczenie nie przekracza Max HP (np. u Leczącej Babki)
    void testLeczenieNiePrzekraczaMaxHp() {
        Gracz g("Bohater", 50, 10); // Gracz ma 50/50 HP

        g.otrzymajObrazenia(15);    // Spada do 35 HP
        QCOMPARE(g.getHp(), 35);

        g.ulecz(100);               // Próbujemy uleczyć go o 100 (potężna Driada)

        QCOMPARE(g.getHp(), 50);    // HP powinno zablokować się na maksymalnym 50!
    }
};

// Obowiązkowe dla systemu QTest (generuje kod meta-obiektów Qt)
#include "testy.moc"
// NOWOŚĆ: Funkcja pomocnicza, którą wywołamy bezpiecznie w main.cpp
int uruchomTestyLogiki() {
    GraTest testy;
    return QTest::qExec(&testy);
}