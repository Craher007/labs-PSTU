#include <iostream>
using namespace std;

int main()
{
    const int n = 10;
    int arr[n];
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    int p, q;
    cin >> p >> q;
    while (p < q) {
        int tmp = arr[q];
        arr[q] = arr[p];
        arr[p] = tmp;
        p++;
        q--;
    }

    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    return 0;
}
