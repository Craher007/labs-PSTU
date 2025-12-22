#include <iostream>
using namespace std;
int main() {
    int a = 10,b=20;
    int *ptr_a = &a, * ptr_b = &b,*tmp;
    tmp = ptr_a;
    ptr_a = ptr_b;
    ptr_b = tmp;
    

    cout << *ptr_a<<" "<<*ptr_b;
}