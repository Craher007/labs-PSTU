

#include <iostream>
#include <random>
#include <ctime>

using namespace std;

int binary_search(int* arr, int size, int number) {

    int first = 0, last = size - 1;
    while (first <= last) {
        int middle = first + (last - first) / 2;
        if (arr[middle] == number)return middle;
        if (arr[middle] > number) last = middle-1;
        else first = middle+1;
    }

    return -1;
}

void sort(int* arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}
int main()
{
    setlocale(LC_ALL, "");
    srand(time(0));
    int size = 10;
    int* arr = new int[size];
    for (int i = 0; i < size; i++) arr[i] = rand() % 100;
    sort(arr, size);

    for (int i = 0; i < size; i++) cout << arr[i] << " ";
    cout << endl;

    int number;
    cout << "Введите число, индекс первого вхождения которого хотите найти: ";
    cin >> number;


    int inde = binary_search(arr, size, number);
    if (inde == -1) cout << "Вхождений числа не найдено";
    else cout << "Вхождение найдено, индекс: " << inde;
    delete[] arr;





}

