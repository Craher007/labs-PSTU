
#include <iostream>
using namespace std;

int main()
{
    const int n = 10;
    int arr[n],tmp[n];
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    int k;
    cin >> k;

    for (int i = 0; i < n; i++) {
        cout << arr[i]<<" ";
    }
    cout << endl;

    for (int i = 0; i < n; i++) {
        int p = i - k;
        if (p < 0) { p += n; }
        tmp[p] = arr[i];
    }
    for (int i = 0; i < n; i++) {
        arr[i] = tmp[i];
        cout << arr[i] << " ";
    }
    return 0;
}
