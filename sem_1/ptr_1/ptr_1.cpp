#include <iostream>
using namespace std;
int main() {
    int a = 10;
    int* ptr_a = &a;

    *ptr_a = 100;

    cout << *ptr_a;
}