

#include <iostream>
#include <random>
#include <ctime>

using namespace std;

int interpolation_search(int* arr, int size, int number) {

    int first = 0, last = size - 1;
    while (arr[first] <= number and number <= arr[last] and first <= last) {
        if ((first == last) or (arr[first] == arr[last])) {
            if (arr[first] == number)return first;
            else return -1;
        }
        int pos = first + ((number - arr[first]) * (last - first)) / (arr[last] - arr[first]);
        if (arr[pos] == number)  return pos;
        else if (number > arr[pos]) first = pos + 1;
        else last = pos - 1;
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


    int inde = interpolation_search(arr, size, number);
    if (inde == -1) cout << "Вхождений числа не найдено";
    else cout << "Вхождение найдено, индекс: " << inde;
    delete[] arr;





}

