#include <iostream>
#include "Pair.h"
#include "money_sum.h"
#include "Object.h"
#include "vector.h"


using namespace std;
int main()
{
    setlocale(LC_ALL, "");

    Vector vc(4);

    Pair pair1;
    Pair pair2;
    cin >> pair1;
    cin >> pair2;
    cout << "Сложение пар: " << pair1 + pair2;

    object* part1 = &pair1;
    object* part2 = &pair2;

    part1->show();
    part2->show();

    money money1;
    money money2;
    cin >> money1;
    cin >> money2;
    cout << endl;
    cout << "Сложение: " << endl << money1 + money2 << endl;
    cout << "Вычитание: " << endl << money1 - money2 << endl;
    cout << "Деление: " << endl << money1 / money2 << endl;
    cout <<endl<< money1 <<endl<< money2;

    object* part3 = &money1;
    object* part4 = &money2;

    part3->show();
    part4->show();

    vc.Add(part1);
    vc.Add(part2);
    vc.Add(part3);
    vc.Add(part4);

    cout << vc;



}

