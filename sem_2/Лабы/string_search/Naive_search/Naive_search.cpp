#include <iostream>
#include <string>

using namespace std;

int Naive_search(string a, string b) {
    int a_size = a.size();
    int b_size = b.size();

    if (a_size < b_size || b_size == 0) return -1;
    for (int i = 0; i <= a_size - b_size; i++) {
        int j = 0;
        while (j < b_size && b[j] == a[i + j]) j++;
        if (j == b_size) return i;
    }
    return -1;
}

int main() {
    setlocale(LC_ALL, "");
    string text;
    string pattern;


    cout << "Введите строку в которой будем производить поиск: ";
    cin >> text;
    cout << endl << "Введите подстроку: ";
    cin >> pattern;

    int index = Naive_search(text, pattern);
    if (index == -1) cout << "Вхождений подстроки не найдено" << endl;
    else cout << "Вхождение подстроки найдено, индекс: " << index << endl;

    return 0;
}