
#include <iostream>
using namespace std;
int main()
{
    const int n = 5;
    int arr[n] = { 7,5,2,67,1 };
    int tmp, min_index;
    for (int i = 0; i < n; i++) {
        min_index = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_index]) { min_index = j; }
        }
        tmp = arr[i];
        arr[i] = arr[min_index];
        arr[min_index] = tmp;
    }
    for (int i = 0; i < n; i++)
    {
        cout << arr[i] << " ";
    }
    return 0;
}

