
#include <iostream>
using namespace std;
int main()
{
    int n, su = 1, a;

    cin >> n;
    if (n == 0) {
        cout << "Невозможно посчитать произведение";
    }
    else {
        for (int i = 1; i <= n; i++) {
            su *= i;
        }
        cout << su;
    }

    return 0;
}

