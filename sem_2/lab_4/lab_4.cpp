#include <iostream>
#include <cstdlib>
using namespace std;

int main()
{
    const int n = 50;
    int range_min = 2;
    int range_max = 280000;
    int arr[n];
    for (int i = 0; i < 50; i++)
    {
        arr[i] = ((double)rand() / RAND_MAX) * (range_max - range_min) +
            range_min;
        cout << arr[i] << " ";
    }
    cout << endl;

    int tmp[n * 2];
    int j = 0;
    for (int i = 0; i < n; i++) {
        if (arr[i] % 7 != 0) {
            tmp[j] = arr[i];
            j++;
        }
        if (arr[i] % 2 != 0 and arr[i] % 7 != 0) {
            tmp[j] = 0;
            j++;
        }
    }
    for (int i = 0; i < j; i++) {
        cout << tmp[i] << " ";
    }
    cout << endl;
}
