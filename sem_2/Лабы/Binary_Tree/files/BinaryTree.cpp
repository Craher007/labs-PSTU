#include "BinaryTree.h"
#include <algorithm>
#include <cstdlib>
#include <cstring>

BinaryTree::BinaryTree() : head(nullptr) {}

BinaryTree::~BinaryTree() {
    freeBranch(head);
}

int BinaryTree::numberOf(const char* value) const {
    return std::atoi(value);
}

void BinaryTree::freeBranch(Node* node) {
    if (!node) {
        return;
    }

    freeBranch(node->leftChild);
    freeBranch(node->rightChild);
    delete node;
}

Node* BinaryTree::addNode(Node* node, const char* value) {
    if (!node) {
        return new Node(value);
    }

    int valueNumber = numberOf(value);
    int nodeNumber = numberOf(node->text);

    if (valueNumber < nodeNumber) {
        node->leftChild = addNode(node->leftChild, value);
    } else if (valueNumber > nodeNumber) {
        node->rightChild = addNode(node->rightChild, value);
    }

    return node;
}

void BinaryTree::add(const char* value) {
    head = addNode(head, value);
}

Node* BinaryTree::minNode(Node* node) const {
    while (node && node->leftChild) {
        node = node->leftChild;
    }

    return node;
}

Node* BinaryTree::eraseNode(Node* node, const char* value) {
    if (!node) {
        return nullptr;
    }

    int valueNumber = numberOf(value);
    int nodeNumber = numberOf(node->text);

    if (valueNumber < nodeNumber) {
        node->leftChild = eraseNode(node->leftChild, value);
    } else if (valueNumber > nodeNumber) {
        node->rightChild = eraseNode(node->rightChild, value);
    } else {
        if (!node->leftChild) {
            Node* rightSide = node->rightChild;
            delete node;
            return rightSide;
        }

        if (!node->rightChild) {
            Node* leftSide = node->leftChild;
            delete node;
            return leftSide;
        }

        Node* next = minNode(node->rightChild);
        char* copy = new char[std::strlen(next->text) + 1];
        std::strcpy(copy, next->text);
        delete[] node->text;
        node->text = copy;
        node->rightChild = eraseNode(node->rightChild, next->text);
    }

    return node;
}

bool BinaryTree::erase(const char* value) {
    if (!contains(value)) {
        return false;
    }

    head = eraseNode(head, value);
    return true;
}

bool BinaryTree::hasNode(Node* node, const char* value) const {
    if (!node) {
        return false;
    }

    int valueNumber = numberOf(value);
    int nodeNumber = numberOf(node->text);

    if (valueNumber == nodeNumber) {
        return true;
    }

    if (valueNumber < nodeNumber) {
        return hasNode(node->leftChild, value);
    }

    return hasNode(node->rightChild, value);
}

bool BinaryTree::contains(const char* value) const {
    return hasNode(head, value);
}

void BinaryTree::readStraight(Node* node, std::vector<const char*>& list) const {
    if (!node) {
        return;
    }

    list.push_back(node->text);
    readStraight(node->leftChild, list);
    readStraight(node->rightChild, list);
}

std::vector<const char*> BinaryTree::straightOrder() const {
    std::vector<const char*> list;
    readStraight(head, list);
    return list;
}

void BinaryTree::readSorted(Node* node, std::vector<const char*>& list) const {
    if (!node) {
        return;
    }

    readSorted(node->leftChild, list);
    list.push_back(node->text);
    readSorted(node->rightChild, list);
}

std::vector<const char*> BinaryTree::sortedOrder() const {
    std::vector<const char*> list;
    readSorted(head, list);
    return list;
}

void BinaryTree::readReverse(Node* node, std::vector<const char*>& list) const {
    if (!node) {
        return;
    }

    readReverse(node->leftChild, list);
    readReverse(node->rightChild, list);
    list.push_back(node->text);
}

std::vector<const char*> BinaryTree::reverseOrder() const {
    std::vector<const char*> list;
    readReverse(head, list);
    return list;
}

int BinaryTree::countByFirst(Node* node, char symbol) const {
    if (!node) {
        return 0;
    }

    int current = node->text[0] == symbol ? 1 : 0;
    return current + countByFirst(node->leftChild, symbol) + countByFirst(node->rightChild, symbol);
}

int BinaryTree::countByFirstSymbol(char symbol) const {
    return countByFirst(head, symbol);
}

Node* BinaryTree::buildMiddle(std::vector<const char*>& list, int left, int right) {
    if (left > right) {
        return nullptr;
    }

    int middle = (left + right) / 2;
    Node* node = new Node(list[middle]);
    node->leftChild = buildMiddle(list, left, middle - 1);
    node->rightChild = buildMiddle(list, middle + 1, right);
    return node;
}

void BinaryTree::makeBalanced() {
    std::vector<const char*> values = sortedOrder();
    Node* newHead = values.empty() ? nullptr : buildMiddle(values, 0, static_cast<int>(values.size()) - 1);
    freeBranch(head);
    head = newHead;
}

void BinaryTree::setPlaces(Node* node, int x, int y, int step) {
    if (!node) {
        return;
    }

    node->screenX = x;
    node->screenY = y;

    int nextStep = std::max(35, step / 2);
    setPlaces(node->leftChild, x - step, y + 80, nextStep);
    setPlaces(node->rightChild, x + step, y + 80, nextStep);
}

void BinaryTree::arrange(int width) {
    if (!head) {
        return;
    }

    setPlaces(head, width / 2, 45, std::max(70, width / 4));
}
