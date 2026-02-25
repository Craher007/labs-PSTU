#include <iostream>
using namespace std;

int main()
{
    const int n = 10;
    int arr[n];
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    int maxm=arr[0],count=1;
    for (int i = 0; i < n; i++) {
        if (maxm < arr[i]) {
            maxm = arr[i];
            count = 1;
        }
        else if (maxm == arr[i]) { count++; }
    }
    cout << "Максимальный элемент: " << maxm << endl;
    cout << "Количество совпадающий элементов: " << count;
    return 0;
}
