#include <iostream>
using namespace std;
int main() {
    float a = 124.2342,b=189.27273;
    float *ptr_a = &a, *ptr_b = &b;
    cout << *ptr_a + *ptr_b;
}