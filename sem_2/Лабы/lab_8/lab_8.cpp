
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <string>
#include <fstream>


using namespace std;
struct Human {
    string lastName="";
    string firstName="";
    string patronymic="";
    int birth=0;
    int height=0;
    int weight=0;
    void Print() 
    {
        cout << "Фамилия: " << lastName<<"\nИмя: " <<firstName<<"\nОтчество: "<<patronymic << "\nДата Рождения: " << birth << "\nРост: " << height << "\nВес: " << weight << endl<<endl;
    }
};

Human* deleteByHeightAndWeight(Human* human, int& size, int Delheight, int Delweight) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (human[i].height == Delheight and human[i].weight == Delweight) {
            count++;
        }
    }

    Human* newhuman = new Human[size - count];

    int j = 0;
    for (int i = 0; i < size; i++) {
        if (human[i].height != Delheight and human[i].weight != Delweight) {
            newhuman[j] = human[i];
            j++;
        }
    }

    delete[] human;
    size -= count;
    return newhuman;
}

Human* AddElement(Human* human, int& size) {
    string lastName, firstName, patronymic; int birth, height, weight;
    cout << "ВВедите данные нового человека:" << endl;
    cout << "Фамилия: "; cin >> lastName;
    cout << "Имя: "; cin >> firstName;
    cout << "Отчество: "; cin >> patronymic;
    cout << "Дата рождения: "; cin >> birth;
    cout << "Рост: "; cin >> height;
    cout << "Вес: "; cin >> weight; cout << endl;

    Human* newHuman = new Human[size + 1];
    for (int i = 0; i < size; i++) {
        newHuman[i] = human[i];
    }
    newHuman[size]= { lastName, firstName, patronymic, birth, height, weight };
    cout << "Добавлнен новый человек: " << endl;;
    newHuman[size].Print();
    return newHuman;

}

int main()
{
    setlocale(LC_ALL, "Russian");

    fstream file("t.txt");
    string post, lastName, firstName, patronymic, birth, height, weight;
    int Couthuman = 10;
    Human* human = new Human[Couthuman];
    for (int i = 0; i < Couthuman; i++) {
        getline(file, lastName);
        getline(file, firstName);
        getline(file, patronymic);
        getline(file, birth);
        getline(file, height);
        getline(file, weight);

        human[i] = { lastName, firstName, patronymic, stoi(birth), stoi(height), stoi(weight) };
        getline(file, post);
    }
    for (int i = 0; i < Couthuman; i++) {
        human[i].Print();
    }
    cout << "Введите Рост и Вес для удаления:" << endl;
    int heightdel, weightdel;
    cin >> heightdel >> weightdel;
    cout << endl;
    human=deleteByHeightAndWeight(human, Couthuman, heightdel, weightdel);
    cout << "Новый список людей:" << endl << endl;
    int n = sizeof(human);
    for (int i = 0; i < n+1; i++) {
        human[i].Print();
    }
    human = AddElement(human, (n));
    n = sizeof(human);
    cout << "Весь список: " << endl << endl;
    for (int i = 0; i < n+1; i++) {
        human[i].Print();
    }
    




}

