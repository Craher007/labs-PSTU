
#include <iostream>
using namespace std;
int main()
{
    int n,a;
    cin >> n;
    if (n < 2) {
        cout << "Ошибка - невозможно составить последовательность чисел" << endl;
    }
    else {
        int maxa;
        cin >>maxa;
        for (int i=1; i < n; i++) {
            cin >> a;
            if (maxa < a) {
                maxa = a;
            }
        }
        cout << maxa;
    }
    

}

