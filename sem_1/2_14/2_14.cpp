
#include <iostream>
using namespace std;
int main()
{
    int n,nsum=0;
    cin >> n;
    while (n != 0) {
        nsum += n % 10;
        n /= 10;
    }
    cout << nsum;
}
