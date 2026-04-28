#include <iostream> 
#include <random>
#include <ctime>
using namespace std;

void Bucket_sort(int* arr, int n) {
    if (n <= 1) return;

    const int bucket_num = 10;
    const int b1_size = 10;

    int bucket[bucket_num][b1_size];
    int bucket_size[bucket_num] = { 0 };

    int maxVal = arr[0];

    for (int i = 1; i < n; i++) {
        if (arr[i] > maxVal) { maxVal = arr[i]; }
    }

    for (int i = 0; i < n; i++) {
        int bucket_index = (bucket_num * arr[i]) / (maxVal + 1);
        if (bucket_size[bucket_index] < b1_size) {
            bucket[bucket_index][bucket_size[bucket_index]] = arr[i];
            bucket_size[bucket_index]++;
        }
    }

    for (int i = 0; i < bucket_num; i++) {
        for (int j = 1; j < bucket_size[i]; j++) {
            int tmp = bucket[i][j];
            int k = j - 1;
            while (k >= 0 && bucket[i][k] > tmp) {
                bucket[i][k + 1] = bucket[i][k];
                k--;
            }
            bucket[i][k + 1] = tmp;
        }
    }

    int index = 0;
    for (int i = 0; i < bucket_num; i++) {
        for (int j = 0; j < bucket_size[i]; j++) {
            arr[index++] = bucket[i][j];
        }
    }
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

    Bucket_sort(arr, size);

    for (int i = 0; i < size; i++) cout << arr[i] << " ";
    cout << endl;
    delete[] arr;
}