#include "mainwindow.h"

#include <QtWidgets>

#include "branchboundsolver.h"
#include "graphcanvas.h"

MainWindow::MainWindow(QWidget* parent) : QWidget(parent) {
    setWindowTitle(QString::fromUtf8("Задача коммивояжера"));
    resize(1040, 650);
    setStyleSheet(
        "QWidget { background: #f4f7fb; color: #202938; }"
        "QPushButton { background: white; border: 1px solid #cfd8e6; border-radius: 6px; padding: 8px 10px; text-align: left; }"
        "QPushButton:hover { border-color: #7d98c8; background: #f9fbff; }"
        "QPushButton:checked { background: #2f67c5; color: white; border-color: #2f67c5; }"
        "QPushButton#solveButton { background: #2f67c5; color: white; font-weight: bold; text-align: center; }"
        "QSpinBox, QTextEdit { background: white; border: 1px solid #cfd8e6; border-radius: 6px; padding: 4px; }"
        "QLabel#sectionLabel { font-weight: bold; color: #334155; background: transparent; }"
        "QLabel#hintLabel { color: #5f6b7a; background: transparent; }"
    );

    canvas = new GraphCanvas(this);
    weightSpin = new QSpinBox(this);
    weightSpin->setRange(1, 999);
    weightSpin->setValue(5);
    logEdit = new QTextEdit(this);
    logEdit->setReadOnly(true);
    logEdit->setMinimumHeight(170);

    QPushButton* moveButton = new QPushButton(QString::fromUtf8("Перемещать"), this);
    QPushButton* edgeButton = new QPushButton(QString::fromUtf8("Связать"), this);
    QPushButton* cutButton = new QPushButton(QString::fromUtf8("Разорвать"), this);
    QPushButton* addButton = new QPushButton(QString::fromUtf8("Добавить вершину"), this);
    QPushButton* demoButton = new QPushButton(QString::fromUtf8("Пример 6 вершин"), this);
    QPushButton* solveButton = new QPushButton(QString::fromUtf8("Решить"), this);
    QPushButton* clearButton = new QPushButton(QString::fromUtf8("Очистить"), this);
    solveButton->setObjectName("solveButton");

    moveButton->setCheckable(true);
    edgeButton->setCheckable(true);
    cutButton->setCheckable(true);
    moveButton->setChecked(true);

    QButtonGroup* modes = new QButtonGroup(this);
    modes->addButton(moveButton, GraphCanvas::MoveMode);
    modes->addButton(edgeButton, GraphCanvas::AddEdgeMode);
    modes->addButton(cutButton, GraphCanvas::RemoveEdgeMode);

    QLabel* modeLabel = new QLabel(QString::fromUtf8("Режим работы:"), this);
    QLabel* weightLabel = new QLabel(QString::fromUtf8("Вес новой связи:"), this);
    QLabel* resultLabel = new QLabel(QString::fromUtf8("Результат:"), this);
    QLabel* hintLabel = new QLabel(QString::fromUtf8("Зажмите левую кнопку на вершине и перетащите ее. В режимах «Связать» и «Разорвать» коротко нажмите две вершины."), this);
    modeLabel->setObjectName("sectionLabel");
    weightLabel->setObjectName("sectionLabel");
    resultLabel->setObjectName("sectionLabel");
    hintLabel->setObjectName("hintLabel");
    hintLabel->setWordWrap(true);

    QVBoxLayout* panel = new QVBoxLayout;
    panel->setSpacing(8);
    panel->addWidget(modeLabel);
    panel->addWidget(moveButton);
    panel->addWidget(edgeButton);
    panel->addWidget(cutButton);
    panel->addSpacing(8);
    panel->addWidget(weightLabel);
    panel->addWidget(weightSpin);
    panel->addWidget(hintLabel);
    panel->addSpacing(8);
    panel->addWidget(addButton);
    panel->addWidget(demoButton);
    panel->addWidget(clearButton);
    panel->addSpacing(8);
    panel->addWidget(solveButton);
    panel->addWidget(resultLabel);
    panel->addWidget(logEdit, 1);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(14, 14, 14, 14);
    layout->setSpacing(14);
    layout->addWidget(canvas, 1);
    layout->addLayout(panel);

    connect(weightSpin, QOverload<int>::of(&QSpinBox::valueChanged), canvas, &GraphCanvas::setEdgeWeight);
    connect(addButton, &QPushButton::clicked, canvas, &GraphCanvas::addNodeAuto);
    connect(demoButton, &QPushButton::clicked, this, [this]() {
        canvas->loadDemo();
        logEdit->clear();
    });
    connect(clearButton, &QPushButton::clicked, this, [this]() {
        canvas->clearGraph();
        logEdit->clear();
    });
    connect(solveButton, &QPushButton::clicked, this, &MainWindow::solve);
    connect(modes, QOverload<int>::of(&QButtonGroup::buttonClicked), this, [this](int id) {
        canvas->setMode(static_cast<GraphCanvas::Mode>(id));
    });

    logEdit->setText(QString::fromUtf8("Готов пример двунаправленного графа на 6 вершинах.\nПеремещайте вершины зажатой левой кнопкой мыши, связывайте пары выбранным весом и нажимайте «Решить»."));
}

void MainWindow::solve() {
    Solution solution = BranchBoundSolver::solve(canvas->graph());
    canvas->setRoute(solution.path);

    if (solution.ok) {
        logEdit->setText(QString::fromUtf8("Оптимальный маршрут: %1\nСтоимость: %2\n\n%3")
            .arg(pathToString(solution.path)).arg(solution.cost).arg(solution.log));
    } else {
        logEdit->setText(solution.log);
    }
}

QString MainWindow::pathToString(const QVector<int>& path) const {
    QStringList parts;
    for (int city : path) {
        parts << QString::number(city + 1);
    }
    return parts.join(QString::fromUtf8(" -> "));
}
