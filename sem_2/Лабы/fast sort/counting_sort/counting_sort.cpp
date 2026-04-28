#include <iostream> 
#include <random>
#include <ctime>
using namespace std;

void Count_sort(int* arr, int size)
{
    if (size <= 1) return;

    int max = arr[0];

    for (int i = 1; i < size; i++) {
        if (arr[i] > max) max = arr[i];
    }

    int* count = new int[max + 1] {0};
    int* output = new int[size];

    for (int i = 0; i < size; i++) {
        count[arr[i]]++;
    }

    for (int i = 1; i <= max; i++) {
        count[i] += count[i - 1];
    }

    for (int i = size - 1; i >= 0; i--) {
        output[count[arr[i]] - 1] = arr[i];
        count[arr[i]]--;
    }

    for (int i = 0; i < size; i++) {
        arr[i] = output[i];
    }

    delete[] count;
    delete[] output;
}

int main() {
    setlocale(LC_ALL, "");
    srand(time(0));
    int size = 10;
    int* arr = new int[size];
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 100;
        cout << arr[i] << " ";
    }
    cout << endl;

    Count_sort(arr, size);

    for (int i = 0; i < size; i++) cout << arr[i] << " ";
    cout << endl;
    delete[] arr;
}