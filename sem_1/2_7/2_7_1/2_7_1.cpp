
#include <iostream>
#include <cmath>

using namespace std;
int main()
{
    setlocale(LC_ALL, "Russian");
    int n;
    cin >> n;
    int k = sqrt(n);
    if (k == sqrt(n)) {
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < k; j++) {
                cout << "*";
            }
            cout << endl;
        }
    }
}