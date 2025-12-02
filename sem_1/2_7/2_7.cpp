

#include <iostream>
#include <cmath>

using namespace std;
int main()
{
    setlocale(LC_ALL, "Russian");
    int tip;
    cout << "Выберите тип квадрата:" << endl << "1 - В котором ВСЕГО N звездочек" << endl << "2 - Заполненный квадрат со стороной N" << endl << "3 - Пустой квадрат со стороной N" << endl;
    cin >> tip;
    int n;

    if (tip!=1 and tip!=2 and tip!=3) {
        cout << "Ввод не распознан" << endl;
    }
    cout << "Введите N" << endl;
    cin >> n;
    if (tip==1) {
        int k = sqrt(n);
        if (k == sqrt(n)) {
            for (int i = 0; i < k;i++) {
                for (int j = 0; j < k; j++) {
                    cout << "*";
                }
                cout << endl;
            }
        }
    }
    else if (tip==2) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << "*";
            }
            cout << endl;
        }
    }
    else {
        for (int i = 0; i < n; i++) {
            cout << "*";
        }
        cout << endl;

        for (int i = 0; i < n-2; i++) {
            cout << "*";
            for (int j = 0; j < n-2; j++) {
                cout << " ";
            }
            cout << "*";
            cout << endl;
        }
        
        for (int i = 0; i < n; i++) {
            cout << "*";
        }
    }
}

