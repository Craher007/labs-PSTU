#pragma once
#include <string>

class Error {
public:
    Error(int code) {
        switch (code) {
        case 1:
            message = "Ошибка: размер вектора больше MAX_SIZE";
            break;
        case 2:
            message = "Ошибка: индекс меньше 0";
            break;
        case 3:
            message = "Ошибка: индекс выходит за границы вектора";
            break;
        case 4:
            message = "Ошибка: размеры векторов не совпадают";
            break;
        case 5:
            message = "Ошибка: итератор выходит за границы вектора";
            break;
        case 6:
            message = "Ошибка: размер вектора не может быть отрицательным";
            break;
        default:
            message = "Неизвестная ошибка";
            break;
        }
    }

    const string& what() const {
        return message;
    }

private:
    string message;
};
