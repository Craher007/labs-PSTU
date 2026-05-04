#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

const string SEP = "/";

int countRuns(const string& name) {
    ifstream f(name);
    int prev, cur, runs = 0;

    if (f >> prev) runs = 1;

    while (f >> cur) {
        if (cur < prev) runs++;
        prev = cur;
    }

    return runs;
}

bool readValue(ifstream& f, int& x) {
    string s;

    if (!(f >> s) || s == SEP) {
        return false;
    }

    x = stoi(s);
    return true;
}

bool writeRun(ifstream& in, ofstream& out, int& saved, bool& hasSaved) {
    int x;

    if (hasSaved) {
        x = saved;
        hasSaved = false;
    }
    else if (!(in >> x)) {
        return false;
    }

    out << x << ' ';

    int prev = x;

    while (in >> x) {
        if (x < prev) {
            saved = x;
            hasSaved = true;
            break;
        }

        out << x << ' ';
        prev = x;
    }

    out << SEP << ' ';
    return true;
}

void mergeRun(ifstream& a, ifstream& b, ofstream& out) {
    int x, y;
    bool hasX = readValue(a, x);
    bool hasY = readValue(b, y);

    while (hasX || hasY) {
        if (!hasY || hasX && x <= y) {
            out << x << ' ';
            hasX = readValue(a, x);
        }
        else {
            out << y << ' ';
            hasY = readValue(b, y);
        }
    }

    out << SEP << ' ';
}

void distribute(const string& source, const string& f1, const string& f2, int runs1, int runs2, int realRuns) {
    ifstream in(source);
    ofstream a(f1), b(f2);

    int emptyRuns = runs1 + runs2 - realRuns;

    while (emptyRuns-- > 0 && runs1-- > 0) {
        a << SEP << ' ';
    }

    int saved = 0;
    bool hasSaved = false;

    for (int i = 0; i < runs1; i++) {
        writeRun(in, a, saved, hasSaved);
    }

    for (int i = 0; i < runs2; i++) {
        writeRun(in, b, saved, hasSaved);
    }
}

void copyResult(const string& from, const string& to) {
    ifstream in(from);
    ofstream out(to);
    string s;

    while (in >> s) {
        if (s != SEP) {
            out << s << ' ';
        }
    }
}

void polyphaseSort(const string& fileName) {
    string file[3] = { fileName, "B.txt", "C.txt" };

    int realRuns = countRuns(fileName);
    if (realRuns <= 1) return;

    int r1 = 1;
    int r2 = 1;

    while (r1 + r2 < realRuns) {
        swap(r1, r2);
        r1 += r2;
    }

    distribute(fileName, file[1], file[2], r1, r2, realRuns);

    int runs[3] = { 0, r1, r2 };
    ifstream in[3];

    in[1].open(file[1]);
    in[2].open(file[2]);

    while (true) {
        int active = 0;
        int result = -1;

        for (int i = 0; i < 3; i++) {
            if (runs[i] > 0) {
                active++;
                result = i;
            }
        }

        if (active == 1 && runs[result] == 1) {
            copyResult(file[result], fileName);
            break;
        }

        int out = -1;
        int a = -1;
        int b = -1;

        for (int i = 0; i < 3; i++) {
            if (runs[i] == 0) {
                out = i;
            }
            else if (a == -1) {
                a = i;
            }
            else {
                b = i;
            }
        }

        int count = min(runs[a], runs[b]);
        ofstream output(file[out]);

        for (int i = 0; i < count; i++) {
            mergeRun(in[a], in[b], output);
        }

        output.close();

        runs[a] -= count;
        runs[b] -= count;
        runs[out] = count;

        if (runs[a] == 0) in[a].close();
        if (runs[b] == 0) in[b].close();

        in[out].clear();
        in[out].open(file[out]);
    }
}

int main() {
    setlocale(LC_ALL, "");
    string fileName = "A.txt";
    ofstream input(fileName);

    int n;

    cout << "Введите размер массива: ";
    cin >> n;

    cout << "Ввод чисел: " << endl;

    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        input << x << ' ';
    }

    input.close();

    polyphaseSort(fileName);

    cout << "Результат сортировки:" << endl;

    ifstream result(fileName);
    int x;

    while (result >> x) {
        cout << x << ' ';
    }

    cout << endl;
}