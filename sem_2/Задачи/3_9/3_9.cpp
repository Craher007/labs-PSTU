
#include <iostream>
using namespace std;

int main()
{
    const int n = 10;
    int arr[n], tmp[n], j = 0;
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    for (int i = 0; i < n; i++) {
        if (arr[i] % 2 == 0) {
            tmp[j] = arr[i];
            j++;
        }
    }
    for (int i = 0; i < j; i++) {
        cout << tmp[i] << " ";
    }
    return 0;
}
