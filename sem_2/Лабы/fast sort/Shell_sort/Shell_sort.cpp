

#include <iostream>
#include <random>
#include <ctime>

using namespace std;

void Shell_sort(int* arr, int size) {
    for (int h = size / 2; h > 0; h /= 2) {
        for (int i = h; i < size; i++) {
            int tmp = arr[i];
            int j;
            for (j = i; j >= h; j -= h) {
                if (tmp < arr[j - h]) arr[j] = arr[j - h];
                else break;
            }
            arr[j] = tmp;
        }
    }
}
int main()
{
    setlocale(LC_ALL, "");
    srand(time(0));
    int size = 10;
    int* arr = new int[size];
    for (int i = 0; i < size; i++) { 
        arr[i] = rand() % 100; 
        cout << arr[i] << " ";
    }
    cout << endl;

    Shell_sort(arr, size);

    for (int i = 0; i < size; i++) cout << arr[i] << " ";
    cout << endl;
    delete[] arr;





}

