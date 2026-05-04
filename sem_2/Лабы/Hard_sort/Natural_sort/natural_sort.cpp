#include <iostream>
#include <fstream>
#include <string>
#include <locale>

using namespace std;

int splitFile(const string& fileName, fstream& A, fstream& B, fstream& C, int blockSize) {
    A.close();
    B.close();
    C.close();

    A.open(fileName, ios::in);
    B.open("B.txt", ios::out);
    C.open("C.txt", ios::out);

    int value;
    int countInBlock = 0;
    int blockCount = 0;
    bool writeToB = true;

    while (A >> value) {
        if (countInBlock == 0) {
            blockCount++;
        }

        if (writeToB) {
            B << value << ' ';
        }
        else {
            C << value << ' ';
        }

        countInBlock++;

        if (countInBlock == blockSize) {
            countInBlock = 0;
            writeToB = !writeToB;
        }
    }

    return blockCount;
}

void writeRemainingBlock( fstream& input, fstream& output, int& value, bool& hasValue, int& used, int blockSize ) {
    while (hasValue && used < blockSize) {
        output << value << ' ';
        used++;
        hasValue = static_cast<bool>(input >> value);
    }
}

int mergeFiles(const string& fileName, fstream& A, fstream& B, fstream& C, int blockSize) {
    A.close();
    B.close();
    C.close();

    A.open(fileName, ios::out);
    B.open("B.txt", ios::in);
    C.open("C.txt", ios::in);

    int valueB;
    int valueC;

    bool hasB = static_cast<bool>(B >> valueB);
    bool hasC = static_cast<bool>(C >> valueC);

    while (hasB || hasC) {
        int usedB = 0;
        int usedC = 0;

        while (hasB && hasC && usedB < blockSize && usedC < blockSize) {
            if (valueB <= valueC) {
                A << valueB << ' ';
                usedB++;
                hasB = static_cast<bool>(B >> valueB);
            }
            else {
                A << valueC << ' ';
                usedC++;
                hasC = static_cast<bool>(C >> valueC);
            }
        }

        writeRemainingBlock(B, A, valueB, hasB, usedB, blockSize);
        writeRemainingBlock(C, A, valueC, hasC, usedC, blockSize);
    }

    return blockSize * 2;
}

void externalSort(const string& fileName) {
    fstream A, B, C;

    int blockSize = 1;
    int blockCount = -1;

    while (blockCount != 1) {
        blockCount = splitFile(fileName, A, B, C, blockSize);

        if (blockCount != 1) {
            blockSize = mergeFiles(fileName, A, B, C, blockSize);
        }
    }

    A.close();
    B.close();
    C.close();
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

    externalSort(fileName);

    cout << "Результат сортировки:" << endl;

    ifstream result(fileName);
    int x;

    while (result >> x) {
        cout << x << ' ';
    }

    cout << endl;

}