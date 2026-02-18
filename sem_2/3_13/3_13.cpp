

#include <iostream>
using namespace std;

int main()
{

    const int n = 4;
    int arr[n][n] = {
        {4,3,2,5},
        {6,2,8,4},
        {1,8,4,8},
        {9,2,7,3}
    };
    int k;
    cin >> k;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << arr[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;



    int tmp;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n - i-1; j++) {
            if (arr[k][j] > arr[k][j + 1]) {
                for (int h = 0; h < n; h++) {
                    tmp = arr[h][j];
                    arr[h][j] = arr[h][j + 1];
                    arr[h][j + 1] = tmp;
                }
            }
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << arr[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

}

