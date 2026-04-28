#include <iostream>
#include <cstdarg>
using namespace std;
int sum(int k, ...) {
    va_list args;
    va_start(args, k);

    int one = va_arg(args, int),f=0;
    for (int i = 0; i < k - 1; i++) {
        int two = va_arg(args, int);
        if (i % 2 == 0) {
            f += one * two;
        }
        else {
            f -= one * two;         
        }
        one = two;

    }
    cout << endl;
    return f;

}

int main()
{
    cout<<sum(5, 1, 2, 3, 4, 5)<<endl;
    cout << sum(10, 1, 2, 3, 4, 5, 8, 3, 7, 2, 7) << endl;
    cout << sum(12, 1, 2, 3, 4, 5, 8, 3, 7, 2, 7, -2, 1) << endl;
}
