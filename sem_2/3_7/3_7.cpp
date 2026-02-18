
#include <iostream>
using namespace std;

int main()
{
    const int n = 10;
    int arr[n];
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    int maxm = arr[0], minm = arr[0];
    for (int i = 0; i < n; i++) {
        if (maxm < arr[i]) { maxm = arr[i]; }
        if (minm > arr[i]) { minm = arr[i]; }
    }
    cout << "Max: " << maxm << endl << "Min: " << minm;


    return 0;
}
