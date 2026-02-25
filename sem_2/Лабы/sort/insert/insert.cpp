

#include <iostream>
using namespace std;
int main()
{
    const int n = 5;
    int arr[n] = { 5,4,3,2,1 };
    int tmp;
    for (int i = 1; i < n; i++) {
        tmp = arr[i];
        int j = i - 1;
        while (j >= 0 and arr[j] > tmp) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = tmp;
    }

    for (int i = 0; i < n; i++)
    {
        cout << arr[i] << " ";
    }

    return 0;

}