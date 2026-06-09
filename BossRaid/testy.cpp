#include <QtTest>
#include "Gracz.h"
#include "Boss.h"

class GraTest : public QObject
{
    Q_OBJECT

private slots:
    void testPancerzPochlaniaObrazenia() {
        Boss p("Testowy Wojownik", 100);
        p.dodajPancerz(20);
        p.otrzymajObrazenia(15);

        QCOMPARE(p.getPancerz(), 5);
        QCOMPARE(p.getHp(), 100);
    }

    void testPrzebiciePancerzaDoHP() {
        Boss p("Testowy Mag", 100);
        p.dodajPancerz(10);
        p.otrzymajObrazenia(25);

        QCOMPARE(p.getPancerz(), 0);
        QCOMPARE(p.getHp(), 85);
    }

    void testPunktyAkcjiGracza() {
        Gracz g("Bohater", 30, 10);

        bool udaneZuzycie = g.zuzyjPA(4);
        QVERIFY(udaneZuzycie == true);
        QCOMPARE(g.getPA(), 6);

        bool zaDrogieZuzycie = g.zuzyjPA(8);
        QVERIFY(zaDrogieZuzycie == false);
        QCOMPARE(g.getPA(), 6);
    }

    void testFazaFuriiBossa() {
        Boss b("Mroczny Test", 100);
        QCOMPARE(b.getFaza(), 1);

        b.otrzymajObrazenia(60);
        b.wykonajAtak(1);

        QCOMPARE(b.getFaza(), 2);
    }

    void testLeczenieNiePrzekraczaMaxHp() {
        Gracz g("Bohater", 50, 10);

        g.otrzymajObrazenia(15);
        QCOMPARE(g.getHp(), 35);

        g.ulecz(100);

        QCOMPARE(g.getHp(), 50);
    }
};

#include "testy.moc"

int uruchomTestyLogiki() {
    GraTest testy;
    return QTest::qExec(&testy);
}