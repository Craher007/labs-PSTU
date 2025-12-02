
#include <iostream>
using namespace std;
int main()
{
    setlocale(LC_ALL, "Russian");
    int n, a;
    cin >> n;
    if (n < 2) {
        cout << "Ошибка - невозможно составить последовательность чисел" << endl;
    }
    else {
        int perv=0;
        for (int i = 0; i < n; i++) {
            cin >> a;
            if (perv == 0) {
                if (a != 0) {
                    if (a < 0) {
                        perv = -1;
                    }
                    else {
                        perv = 1;
                    }
                }
            }  
        }
        if (perv > 0) {
            cout << "Первое ввели - положительное число";
        }
        else if (perv < 0) {
            cout << "Первое ввели - отрицательное число";
        }
        else {
            cout << "Все числа в последовательности нулевые";
        }
    }


}

