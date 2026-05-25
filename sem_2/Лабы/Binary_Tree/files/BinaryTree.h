#ifndef BINARYTREE_H
#define BINARYTREE_H

#include "Node.h"
#include <vector>

class BinaryTree {
private:
    Node* head;

    int numberOf(const char* value) const;
    Node* addNode(Node* node, const char* value);
    Node* eraseNode(Node* node, const char* value);
    Node* minNode(Node* node) const;
    bool hasNode(Node* node, const char* value) const;
    void readStraight(Node* node, std::vector<const char*>& list) const;
    void readSorted(Node* node, std::vector<const char*>& list) const;
    void readReverse(Node* node, std::vector<const char*>& list) const;
    int countByFirst(Node* node, char symbol) const;
    Node* buildMiddle(std::vector<const char*>& list, int left, int right);
    void freeBranch(Node* node);
    void setPlaces(Node* node, int x, int y, int step);

public:
    BinaryTree();
    ~BinaryTree();

    void add(const char* value);
    bool erase(const char* value);
    bool contains(const char* value) const;
    std::vector<const char*> straightOrder() const;
    std::vector<const char*> sortedOrder() const;
    std::vector<const char*> reverseOrder() const;
    int countByFirstSymbol(char symbol) const;
    void makeBalanced();
    void arrange(int width);
    Node* root() const { return head; }
};

#endif
