#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>

using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");
    fstream file("F1.txt");
    string line;
    int CountLine = 15, k, Kcount = 0;
    cout << "Введите номер строки, с которой потребуется копирование(до 10): "; cin >> k;
    ofstream out;
    out.open("F2.txt");
    for (int i = 1; i <= CountLine; i++) {
        getline(file, line);
        if (i == k or (Kcount <= 5 and Kcount >= 1)) {
            out << line << endl;
            Kcount++;
        }
    }

    const int N = 14;
    char vowel[N] = { 'A', 'a', 'E', 'e', 'I', 'i', 'J', 'j', 'O', 'o', 'U', 'u', 'Y', 'y' };
    fstream file2("F2.txt");
    int CountVowel = 0, CountFile2 = 6;
    string Line2;
    for (int n = 0; n < CountFile2; n++) {
        getline(file2, Line2);
        for (int i = 0; Line2[i]; i++)
        {
            for (int j = 0; j < N; j++)
            {
                if (Line2[i] == vowel[j])
                    CountVowel++;
            }
        }
    }
    cout << "Количество гласных в файле 'F2.txt': " << CountVowel;

}

