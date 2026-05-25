#ifndef NODE_H
#define NODE_H

#include <cstring>

class Node {
public:
    char* text;
    Node* leftChild;
    Node* rightChild;
    int screenX;
    int screenY;

    explicit Node(const char* value) : text(new char[std::strlen(value) + 1]),
        leftChild(nullptr),
        rightChild(nullptr),
        screenX(0),
        screenY(0) {
        std::strcpy(text, value);
    }

    ~Node() {
        delete[] text;
    }
};

#endif
