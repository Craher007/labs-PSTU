#include <iostream>
using namespace std;
int main()
{
    const int n = 5;
    int tmp;
    int arr[n] = { 7,5,2,5,1 };
    
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }

    }

    for (int i = 0; i < n; i++)
    {
        cout << arr[i] << " ";
    }
    return 0;
}

