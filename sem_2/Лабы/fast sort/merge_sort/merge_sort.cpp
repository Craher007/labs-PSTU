#include <iostream> 
#include <random>
#include <ctime>
using namespace std;

void merge(int* arr, int left, int mid, int right) {
    int LA = mid - left + 1;
    int RA = right - mid;

    int* left_arr = new int[LA];
    int* right_arr = new int[RA];

    for (int i = 0; i < LA; i++) { left_arr[i] = arr[left + i]; }
    for (int i = 0; i < RA; i++) { right_arr[i] = arr[mid + 1 + i]; }

    int leftidx = 0;
    int rightidx = 0;
    int mergeidx = left;

    while (leftidx < LA && rightidx < RA) {
        if (left_arr[leftidx] <= right_arr[rightidx]) {
            arr[mergeidx] = left_arr[leftidx];
            leftidx++;
        }
        else {
            arr[mergeidx] = right_arr[rightidx];
            rightidx++;
        }
        mergeidx++;
    }

    while (leftidx < LA) {
        arr[mergeidx] = left_arr[leftidx];
        leftidx++;
        mergeidx++;
    }

    while (rightidx < RA) {
        arr[mergeidx] = right_arr[rightidx];
        rightidx++;
        mergeidx++;
    }

    delete[] left_arr;
    delete[] right_arr;
}

void Merge_sort(int* arr, int start, int end) {
    if (start >= end) return;
    int mid = start + (end - start) / 2;

    Merge_sort(arr, start, mid);
    Merge_sort(arr, mid + 1, end);
    merge(arr, start, mid, end);
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

    Merge_sort(arr, 0, size - 1);

    for (int i = 0; i < size; i++) cout << arr[i] << " ";
    cout << endl;
    delete[] arr;
}