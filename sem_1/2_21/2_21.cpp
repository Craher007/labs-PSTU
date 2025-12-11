
#include <iostream>
using namespace std;
int main()
{
    setlocale(LC_ALL, "Russian");
    bool flag = true;
    int n1,n2;
    cin >> n1;
    while (n1 != 0) {
        cin >> n2;
        if (n1 > n2 and n2!=0) { flag = false; }
        n1 = n2;
    }
    if (flag==true) { cout << "Числа упорядочены по возрастанию"; }
    else { cout << "Числа не упоряжочены по возрастанию"; }
}
