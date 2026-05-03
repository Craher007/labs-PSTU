#include <iostream>
#include <string>

using namespace std;

int Naive_search(string text, string ptn) {
    int size_line = text.size();
    int size_ptn = ptn.size();

    if (size_line < size_ptn || size_ptn == 0) return -1;
    for (int i = 0; i <= size_line - size_ptn; i++) {
        int j = 0;
        while (j < size_ptn && ptn[j] == text[i + j]) j++;
        if (j == size_ptn) return i;
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
}