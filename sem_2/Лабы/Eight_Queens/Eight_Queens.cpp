#include <iostream>

using namespace std;

const int size_arr = 8;
int a[size_arr][size_arr]{};
int tryCount = 0;
void show() {
    for (int i = 0; i < size_arr; i++) {
        for (int j = 0; j < size_arr; j++) {
            cout << (a[i][j] ? "O " : "X ");
        }
        cout << endl;
    }
}
bool test(int row, int col) {
    for (int i = 0; i < row; i++) {
        if (a[i][col]) { return false; }
    }
    for (int i = 1; row - i >= 0 and col - i >= 0; i++) {
        if (a[row - i][col - i]) { return false; }
    }
    for (int i = 1; row - i >= 0 and col + i < size_arr; i++) {
        if (a[row - i][col + i]) { return false; }
    }
    return true;
}
void game(int row) {
    if (row == size_arr) {
        cout << "Try #" << ++tryCount << endl << endl;
        show();
        cout << endl;
        return;
    }
    for (int col = 0; col < size_arr; col++) {
        if (test(row, col)) {
            a[row][col] = 1;
            game(row + 1);
            a[row][col] = 0;
        }
    }
}
int main() {
    game(0);
    return 0;
}