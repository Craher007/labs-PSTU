
#include <iostream>
#include <string>
using namespace std;
int main()
{
    string str,start,tmp;
    getline(cin, str,'.');
    str += ' ';
    int j = 0;
    while (str[j] != ' ') {
        start += str[j];
        j++;
    }
    for (int i = j+1; i < str.length(); i++) {
        if (str[i] != ' ') {
            tmp += str[i];
        }
        else {
            if (tmp == start) {
                cout << tmp<<" ";
            }
            tmp = "";
        }
    }

    
}
