#include <iostream>
#include <string>

using namespace std;

int Boyer_Moore_search(string text, string ptn) {
    int size_line = text.size();
    int size_ptn = ptn.size();

    if (size_line == 0 || size_line < size_ptn)  return -1;

    const int ALPHABET_SIZE = 256;
    int alpha[ALPHABET_SIZE];

    for (int i = 0; i < ALPHABET_SIZE; ++i) alpha[i] = -1;

    for (int i = 0; i < size_ptn; ++i) alpha[ptn[i]] = i;

    int* suff = new int[size_ptn];
    int* shift = new int[size_ptn];

    suff[size_ptn - 1] = size_ptn;
    int x = size_ptn - 1, y = size_ptn - 1;
    for (int i = size_ptn - 2; i >= 0; --i) {
        if (i > x && suff[i + size_ptn - 1 - y] < i - x) suff[i] = suff[i + size_ptn - 1 - y];
        else {
            if (i < x) x = i;
            y = i;
            while (x >= 0 && ptn[x] == ptn[size_ptn - 1 - (y - x)]) x--;
            suff[i] = y - x;
        }
    }
    for (int i = 0; i < size_ptn; i++) shift[i] = size_ptn;

    int j = 0;
    for (int i = size_ptn - 1; i >= 0; i--) {
        if (suff[i] == i + 1) {
            for (; j < size_ptn - 1 - i; j++) {
                if (shift[j] == size_ptn) {
                    shift[j] = size_ptn - 1 - i;
                }
            }
        }
    }
    for (int i = 0; i <= size_ptn - 2; i++) {
        shift[size_ptn - 1 - suff[i]] = size_ptn - 1 - i;
    }

    int pos = 0;
    while (pos <= size_line - size_ptn) {
        int j = size_ptn - 1;

        while (j >= 0 && ptn[j] == text[pos + j]) j--;
        if (j < 0) {
            delete[] suff;
            delete[] shift;
            return pos;
        }
        int badShift = j - alpha[text[pos + j]];

        if (badShift < 1) badShift = 1;

        int goodShift = shift[j];
        pos += (badShift > goodShift) ? badShift : goodShift;
    }

    delete[] suff;
    delete[] shift;
    return -1;

}

int main(){
    setlocale(LC_ALL, "");
    string text;
    string pattern;


    cout << "Введите строку в которой будем производить поиск: ";
    cin >> text;
    cout << endl << "Введите подстроку: ";
    cin >> pattern;

    int index = Boyer_Moore_search(text, pattern);
    if (index == -1) cout << "Вхождений подстроки не найдено" << endl;
    else cout << "Вхождение подстроки найдено, индекс: " << index << endl;

    return 0;
}