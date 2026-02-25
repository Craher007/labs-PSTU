

#include <iostream>
using namespace std;

int main()
{
    const int n = 5;
    int arr[n][n] = {
        {1,2,3,4,5},
        {6,7,8,9,0},
        {10,11,12,13,14},
        {15,16,17,18,19},
        {20,21,22,23,24}
    };
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << arr[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    int tmp;
    for (int i = 0; i < n; i++) {
        arr[i][n-1-i] = 0;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << arr[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

}

