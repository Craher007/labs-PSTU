#include <iostream>
using namespace std;
int main() {
    int n, fact=1;
    cin >> n;
    int* ptr_fact = &fact;
    for (int i = 1; i <= n; i++) {
        *ptr_fact *= i;
    }
    cout << *ptr_fact;
}