
#include <iostream>
using namespace std;

void han(int n, int start, int point, int temp) {
    if (n <= 0) {
        return;
    }
    han(n - 1, start, temp, point);
    cout << start << "=>" << point << endl;
    han(n - 1, temp, point, start);
}


int main()
{
    han(3,1,3,2);
}
