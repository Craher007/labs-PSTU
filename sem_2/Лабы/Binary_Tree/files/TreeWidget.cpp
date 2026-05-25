#include "TreeWidget.h"
#include <QFont>
#include <QPainter>
#include <QPaintEvent>

TreeWidget::TreeWidget(QWidget* parent) : QWidget(parent), currentTree(nullptr) {}

void TreeWidget::showTree(BinaryTree* tree) {
    currentTree = tree;

    if (currentTree) {
        currentTree->arrange(width());
    }

    update();
}

void TreeWidget::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::white);

    if (!currentTree || !currentTree->root()) {
        painter.setPen(Qt::darkGray);
        painter.setFont(QFont("Arial", 14));
        painter.drawText(rect(), Qt::AlignCenter, "Дерево пустое");
        return;
    }

    currentTree->arrange(width());
    drawBranch(painter, currentTree->root());
    drawCircle(painter, currentTree->root());
}

void TreeWidget::drawBranch(QPainter& painter, Node* node) {
    if (!node) {
        return;
    }

    painter.setPen(QPen(Qt::darkGray, 2));

    if (node->leftChild) {
        painter.drawLine(node->screenX, node->screenY, node->leftChild->screenX, node->leftChild->screenY);
        drawBranch(painter, node->leftChild);
    }

    if (node->rightChild) {
        painter.drawLine(node->screenX, node->screenY, node->rightChild->screenX, node->rightChild->screenY);
        drawBranch(painter, node->rightChild);
    }
}

void TreeWidget::drawCircle(QPainter& painter, Node* node) {
    if (!node) {
        return;
    }

    drawCircle(painter, node->leftChild);
    drawCircle(painter, node->rightChild);

    QRect place(node->screenX - 24, node->screenY - 18, 48, 36);
    painter.setPen(QPen(Qt::black, 2));
    painter.setBrush(QColor(220, 235, 255));
    painter.drawEllipse(place);

    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 11, QFont::Bold));
    painter.drawText(place, Qt::AlignCenter, QString::fromUtf8(node->text));
}
