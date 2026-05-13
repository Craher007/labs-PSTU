#include "Dialog.h"

#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

Dialog::Dialog(): Vector(), EndState(0){}

void Dialog::GetEvent(TEvent& event)
{
    string commandLine;
    string operations = "+-szmq";

    cout << endl<< "Команды: ";
    getline(cin >> ws, commandLine);

    if (commandLine.empty())
    {
        event.what = evNothing;
        return;
    }

    char code = commandLine[0];
    if (operations.find(code) != string::npos)
    {
        event.what = evMessage;

        switch (code)
        {
        case 'm':
            event.command = cmMake;
            break;
        case '+':
            event.command = cmAdd;
            break;
        case '-':
            event.command = cmDel;
            break;
        case 's':
            event.command = cmShow;
            break;
        case 'z':
            event.command = cmGet;
            break;
        case 'q':
            event.command = cmQuit;
            break;
        }

        string param = commandLine.substr(1);
        if (!param.empty()) event.a = atoi(param.c_str());
        else event.a = 0;
    }
    else event.what = evNothing;
}

int Dialog::Execute()
{
    TEvent event;
    do
    {
        EndState = 0;
        GetEvent(event);
        HandleEvent(event);
    } while (!Valid());

    return EndState;
}

void Dialog::HandleEvent(TEvent& event)
{
    if (event.what != evMessage) return;

    switch (event.command)
    {
    case cmMake:
        if (event.a <= 0)
        {
            cout << "Размер группы должен быть больше нуля" << endl;
            ClearEvent(event);
            break;
        }

        for (int i = 0; i < cur; ++i) delete beg[i];
        delete[] beg;

        beg = new Object*[event.a];
        size = event.a;
        cur = 0;
        for (int i = 0; i < size; ++i)
        {
            beg[i] = nullptr;
        }
        ClearEvent(event);
        break;
    case cmAdd:
        Add();
        ClearEvent(event);
        break;
    case cmDel:
        Del();
        ClearEvent(event);
        break;
    case cmShow:
        Show();
        ClearEvent(event);
        break;
    case cmQuit:
        EndExec();
        ClearEvent(event);
        break;
    default:
        Vector::HandleEvent(event);
        ClearEvent(event);
        break;
    }
}
