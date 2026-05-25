#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "BinaryTree.h"

QT_BEGIN_NAMESPACE
class QLineEdit;
class QTextEdit;
class TreeWidget;
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void addValue();
    void deleteValue();
    void findValue();
    void balanceValues();
    void countFirstSymbol();
    void showStraightOrder();
    void showSortedOrder();
    void showReverseOrder();

private:
    BinaryTree tree;
    TreeWidget* treeView;
    QLineEdit* valueEdit;
    QLineEdit* symbolEdit;
    QTextEdit* journal;

    bool readValue(QString& value);
    QString formatList(const QString& title, const std::vector<const char*>& values) const;
    void refreshTree();
    void printLine(const QString& text);
};

#endif
