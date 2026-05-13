#include "Dialog.h"

#include <iostream>
using namespace std;
int main()
{
    setlocale(LC_ALL, "");
    cout << "Команды:" << endl;
    cout << "m <size>  - Создание группы" << endl;
    cout << "+         - Добавление элемента" << endl;
    cout << "-         - Удаление последнего элемента" << endl;
    cout << "s         - Вывод группы" << endl;
    cout << "z         - Вывод имен" << endl;
    cout << "q         - Выход" << endl;

    Dialog dialog;
    dialog.Execute();

    return 0;
}
