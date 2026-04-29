#include <iostream>
#include <string>

using namespace std;

int KMP_search(string text, string ptn) {
    int size_line = text.size();
    int size_ptn = ptn.size();

    if (size_line == 0 || size_line < size_ptn) return -1;

    int* pref = new int[size_ptn];

    pref[0] = 0;
    for (int i = 1, j = 0; i < size_ptn; i++) {
        while (j > 0 && ptn[i] != ptn[j])  j = pref[j - 1];
        if (ptn[i] == ptn[j]) j++;
        pref[i] = j;
    }

    for (int i = 0, j = 0; i < size_line; i++) {
        while (j > 0 && text[i] != ptn[j]) j = pref[j - 1];
        if (text[i] == ptn[j]) j++;
        if (j == size_ptn) {
            delete[] pref;
            return i - size_ptn + 1;
        }
    }

    delete[] pref;
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

    int index = KMP_search(text, pattern);
    if (index == -1) cout << "Вхождений подстроки не найдено" << endl;
    else cout << "Вхождение подстроки найдено, индекс: " << index << endl;

    return 0;
}