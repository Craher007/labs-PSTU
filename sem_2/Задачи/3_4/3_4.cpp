#include <iostream>
using namespace std;

int main()
{
    const int n = 10;
    int arr[n];
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    bool sorted = true;
    for (int i = 0; i < n-1; i++) {
        if (arr[i] >= arr[i + 1]) {
            sorted = false;
            break;
        }
    }
    if (sorted) {cout << "Упорядочен по возрастанию";}
    else {cout << "Не упорядочен по возрастанию";}
    return 0;
}
