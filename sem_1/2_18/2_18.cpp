#include <iostream>
#include <sstream> 
using namespace std;
int main()
{
    int n;
    string nobr;
    cin >> n;
    while (n != 0) {
        nobr += to_string(n % 10);
        n /= 10;
    }
    cout << nobr;
}

