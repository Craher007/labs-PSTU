#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QWidget>
#include "BinaryTree.h"

class TreeWidget : public QWidget {
    Q_OBJECT

public:
    explicit TreeWidget(QWidget* parent = nullptr);
    void showTree(BinaryTree* tree);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    BinaryTree* currentTree;

    void drawBranch(QPainter& painter, Node* node);
    void drawCircle(QPainter& painter, Node* node);
};

#endif
