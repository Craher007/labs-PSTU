
#include <iostream>
using namespace std;
int main()
{
    int n,star=1;

    cin >> n;
    int space = (n ) / 2;

    if (n % 2 == 0) {
        cout << "Число четное, гнарисовать равнобедренный треугольник не поолучится!";
        return 0;
    }
    for (int i = 1; i <= (n+1)/2; i++) {
        for (int j = 0; j < space; j++) {
            cout << " ";

        }
        for (int j = 0; j < star; j++) {
            cout << "*";
        }
        cout << endl;
        star += 2;
        space -=1;
        
        
    }
    return 0;
}

