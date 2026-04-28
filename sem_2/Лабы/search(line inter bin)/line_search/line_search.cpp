
#include <iostream>
#include <random>
#include <ctime>

using namespace std;

int line_search(int* arr, int size, int number) {
	for (int i = 0; i < size; i++) {
		if (arr[i] == number) return i;
	}
	return -1;
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
	int number;
	cout << "Введите число, индекс первого вхождения которого хотите найти: ";
	cin >> number;


	int inde = line_search(arr, size, number);
	if (inde == -1) cout << "Вхождений числа не найдено";
	else cout << "Вхождение найдено, индекс: "<< inde;
	delete[] arr;

}

