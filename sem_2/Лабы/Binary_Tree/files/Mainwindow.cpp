#include "Mainwindow.h"
#include "TreeWidget.h"
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QStringList>
#include <QTextEdit>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    QWidget* page = new QWidget(this);
    QVBoxLayout* pageLayout = new QVBoxLayout(page);
    pageLayout->setSpacing(12);
    pageLayout->setContentsMargins(14, 14, 14, 14);
    setCentralWidget(page);

    treeView = new TreeWidget(this);
    treeView->setMinimumSize(780, 430);
    pageLayout->addWidget(treeView, 1);

    QGroupBox* controlBox = new QGroupBox("Управление деревом", this);
    QGridLayout* controls = new QGridLayout(controlBox);
    controls->setHorizontalSpacing(10);
    controls->setVerticalSpacing(8);

    valueEdit = new QLineEdit(this);
    valueEdit->setPlaceholderText("Целое число");
    symbolEdit = new QLineEdit(this);
    symbolEdit->setMaxLength(1);
    symbolEdit->setPlaceholderText("1 символ");

    QPushButton* addButton = new QPushButton("Добавить", this);
    QPushButton* deleteButton = new QPushButton("Удалить", this);
    QPushButton* findButton = new QPushButton("Найти", this);
    QPushButton* balanceButton = new QPushButton("Сбалансировать", this);
    QPushButton* countButton = new QPushButton("Подсчитать", this);
    QPushButton* straightButton = new QPushButton("Прямой обход", this);
    QPushButton* sortedButton = new QPushButton("Симметричный обход", this);
    QPushButton* reverseButton = new QPushButton("Обратный обход", this);

    controls->addWidget(new QLabel("Ключ:", this), 0, 0);
    controls->addWidget(valueEdit, 0, 1, 1, 2);
    controls->addWidget(addButton, 0, 3);
    controls->addWidget(deleteButton, 0, 4);
    controls->addWidget(findButton, 0, 5);
    controls->addWidget(balanceButton, 0, 6);

    controls->addWidget(new QLabel("Первый символ:", this), 1, 0);
    controls->addWidget(symbolEdit, 1, 1);
    controls->addWidget(countButton, 1, 2);
    controls->addWidget(straightButton, 1, 3);
    controls->addWidget(sortedButton, 1, 4, 1, 2);
    controls->addWidget(reverseButton, 1, 6);

    pageLayout->addWidget(controlBox);

    journal = new QTextEdit(this);
    journal->setReadOnly(true);
    journal->setMaximumHeight(130);
    journal->setPlaceholderText("Здесь появятся результаты операций");
    pageLayout->addWidget(journal);

    connect(addButton, &QPushButton::clicked, this, &MainWindow::addValue);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteValue);
    connect(findButton, &QPushButton::clicked, this, &MainWindow::findValue);
    connect(balanceButton, &QPushButton::clicked, this, &MainWindow::balanceValues);
    connect(countButton, &QPushButton::clicked, this, &MainWindow::countFirstSymbol);
    connect(straightButton, &QPushButton::clicked, this, &MainWindow::showStraightOrder);
    connect(sortedButton, &QPushButton::clicked, this, &MainWindow::showSortedOrder);
    connect(reverseButton, &QPushButton::clicked, this, &MainWindow::showReverseOrder);
    connect(valueEdit, &QLineEdit::returnPressed, this, &MainWindow::addValue);

    setWindowTitle("Бинарное дерево поиска");
    resize(960, 680);
    refreshTree();
}

bool MainWindow::readValue(QString& value) {
    value = valueEdit->text().trimmed();

    if (value.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите ключ.");
        return false;
    }

    bool ok = false;
    value.toInt(&ok);

    if (!ok) {
        QMessageBox::warning(this, "Ошибка", "Ключ должен быть целым числом.");
        return false;
    }

    return true;
}

QString MainWindow::formatList(const QString& title, const std::vector<const char*>& values) const {
    QStringList parts;

    for (const char* value : values) {
        parts << QString::fromUtf8(value);
    }

    if (parts.isEmpty()) {
        return title + ": пусто";
    }

    return title + ": " + parts.join(" ");
}

void MainWindow::refreshTree() {
    treeView->showTree(&tree);
}

void MainWindow::printLine(const QString& text) {
    journal->append(text);
}

void MainWindow::addValue() {
    QString value;

    if (!readValue(value)) {
        return;
    }

    QByteArray bytes = value.toUtf8();

    if (tree.contains(bytes.constData())) {
        printLine("Такой ключ уже есть: " + value);
    } else {
        tree.add(bytes.constData());
        printLine("Добавлен ключ: " + value);
        refreshTree();
    }

    valueEdit->clear();
}

void MainWindow::deleteValue() {
    QString value;

    if (!readValue(value)) {
        return;
    }

    QByteArray bytes = value.toUtf8();

    if (tree.erase(bytes.constData())) {
        printLine("Удален ключ: " + value);
        refreshTree();
    } else {
        printLine("Ключ не найден: " + value);
    }

    valueEdit->clear();
}

void MainWindow::findValue() {
    QString value;

    if (!readValue(value)) {
        return;
    }

    QByteArray bytes = value.toUtf8();
    printLine(tree.contains(bytes.constData()) ? "Ключ найден: " + value : "Ключ не найден: " + value);
    valueEdit->clear();
}

void MainWindow::balanceValues() {
    tree.makeBalanced();
    refreshTree();
    printLine("Дерево сбалансировано.");
}

void MainWindow::countFirstSymbol() {
    QString symbolText = symbolEdit->text().trimmed();

    if (symbolText.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите символ.");
        return;
    }

    char symbol = symbolText.at(0).toLatin1();
    int amount = tree.countByFirstSymbol(symbol);
    printLine("Ключей, начинающихся с '" + symbolText.left(1) + "': " + QString::number(amount));
}

void MainWindow::showStraightOrder() {
    printLine(formatList("Прямой обход", tree.straightOrder()));
}

void MainWindow::showSortedOrder() {
    printLine(formatList("Симметричный обход", tree.sortedOrder()));
}

void MainWindow::showReverseOrder() {
    printLine(formatList("Обратный обход", tree.reverseOrder()));
}
