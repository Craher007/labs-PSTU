
#include <iostream>
using namespace std;

int main()
{
    int n = 7,index_del,index_up,k;
    int* arr = new int[n] {6,4,7,3,6,3,2};
    cin >> index_del >> k;
    int* New_arr = new int[n-k] {};
    for (int i = 0; i < index_del; i++) {
        New_arr[i] = arr[i];
    }
    for (int i = index_del+k; i < n; i++) {
        New_arr[i-k] = arr[i];
    }

    for (int i = 0; i < n-k; i++) {
        cout << New_arr[i] << " ";
    }
    cout << endl;

    cin >> index_up;

    for (int i = 0; i < index_up; i++) {
        arr[i]=New_arr[i];
    }
    for (int i = index_up; i < index_up+k; i++) {
        arr[i] = 1;
    }
    for (int i = index_up + k; i < n; i++) {
        arr[i] = New_arr[i-k];
    }

    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    delete[] New_arr;
    delete[] arr;


}
