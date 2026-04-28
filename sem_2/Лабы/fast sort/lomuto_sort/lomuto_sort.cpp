

#include <iostream>
#include <random>
#include <ctime>

using namespace std;

int partition(int* arr, int first, int last) {
    int pivot = arr[last];
    int i = first - 1;

    for (int j = first; j <= last - 1; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i+1], arr[last]);
    return(i + 1);
}

void lomuto_sort(int* arr, int first, int last) {
    if (first < last) {
        int pI = partition(arr, first, last);

        lomuto_sort(arr, first, pI-1);
        lomuto_sort(arr, pI + 1, last);
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

    lomuto_sort(arr, 0, size - 1);

    for (int i = 0; i < size; i++) cout << arr[i] << " ";
    cout << endl;
    delete[] arr;





}

