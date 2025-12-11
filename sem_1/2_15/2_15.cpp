
#include <iostream>
using namespace std;
int main()
{
    int n, s;
    cin >> n >> s;
    bool flag = false;
    while ( !flag and n != 0) {
        if ((n%10) == s) { flag = true; }
        n /= 10;
    }
    if (flag) { cout << "Число " << s << " входит в число " << n; }
    else { cout << "Число " << s << " не входит в число " << n; }
}

