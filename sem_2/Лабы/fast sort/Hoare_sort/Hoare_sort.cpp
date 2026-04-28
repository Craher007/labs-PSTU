

#include <iostream>
#include <random>
#include <ctime>

using namespace std;

int partition(int* arr, int first, int last) {
    int pivot = arr[first + (last - first) / 2];
    int i = first - 1;
    int j = last + 1;

    while (true) {
        do { i++; } while (arr[i] < pivot);
        do { j--; } while (arr[j] > pivot);

        if (i >= j) return j;

        swap(arr[i], arr[j]);
    }
}

void Hoare_sort(int* arr, int first, int last) {
    if (first < last) {
        int pI = partition(arr, first, last);

        Hoare_sort(arr, first, pI);
        Hoare_sort(arr, pI + 1, last);
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

    Hoare_sort(arr, 0,size-1);

    for (int i = 0; i < size; i++) cout << arr[i] << " ";
    cout << endl;
    delete[] arr;





}

