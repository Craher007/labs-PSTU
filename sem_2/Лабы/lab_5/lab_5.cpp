#include <iostream>
#include <cstdlib>
using namespace std;

const int n = 5;


int mas(int arr[n][n]) {
    for (int i = 0; i < n; i++) {
        int res_last = 0;
        for (int j = i+1; j < n; j++) {
            int m = 0, h = 0;
            for (int k = 0; k < n; k++) {
                m += arr[i][k] * arr[j][k];
                h += arr[i][k] * arr[i][k];
            }
            if (m != 0 or h != 1) { 
                cout << "Матрица не ортонормированная";
                return 0;
            }

        }
        if (i == n - 1) {
            int res_last = 0;
            for (int j = 0; j < n; j++) {
                res_last += arr[i][j] * arr[i][j];
            }
            if (res_last!=1) {
                cout << "Матрица не ортонормированная";
                return 0;
            }
        }
        
    }
    cout << "Матрица ортонормированная";
    return 0;
}




int main()
{
    const int h = 5;
    int arr[h][h] = {
        {1,0,0,0,0},
        {0,1,0,0,0},
        {0,0,1,0,0},
        {0,0,0,1,0},
        {0,0,0,0,1}
    };
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < h; j++) {
            cout << arr[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    mas(arr);
}
