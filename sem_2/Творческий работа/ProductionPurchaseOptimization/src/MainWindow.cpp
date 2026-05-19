#include "MainWindow.h"
#include "DateUtils.h"
#include "ItemDialog.h"
#include "Report.h"
#include <QAbstractItemView>
#include <QApplication>
#include <QDate>
#include <QFormLayout>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QTabWidget>
#include <QTimer>
#include <QVBoxLayout>
#include <algorithm>
#include <cmath>

using namespace std;

static void enableToggleSelection(QTableWidget* table) {
    // Таблицы результата нужны только для просмотра.
    // Повторный клик по выделенной ячейке снимает выделение.
    QObject::connect(table, &QTableWidget::itemPressed, table, [table](QTableWidgetItem* item) {
        if (item != nullptr && item->isSelected()) {
            QTimer::singleShot(0, table, [table]() {
                table->clearSelection();
            });
        }
    });
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), database("database.txt") {
    // Загружаем файл базы один раз при старте и сразу обновляем оба раздела интерфейса.
    database.load();
    buildUi();
    refreshItems();
    refreshProductList();
}

QString MainWindow::formatDouble(double value) const {
    if (fabs(value - round(value)) < 0.0001) {
        return QString::number(static_cast<long long>(round(value)));
    }

    return QString::number(value, 'f', 2);
}

QString MainWindow::formatDateQt(const Date& date) const {
    return QString::fromStdString(formatDate(date));
}

Date MainWindow::fromQDate(const QDate& date) const {
    Date result;
    result.day = date.day();
    result.month = date.month();
    result.year = date.year();
    return result;
}

bool MainWindow::pieceUnit(const QString& unit) const {
    QString value = unit.trimmed().toLower();
    return value == "шт" || value == "штук" || value == "штука" || value == "штуки";
}

bool MainWindow::wholeNumber(double value) const {
    return fabs(value - round(value)) < 0.0001;
}

void MainWindow::buildUi() {
    setWindowTitle("Оптимизация производственного закупа");
    resize(1400, 900);
    setMinimumSize(1180, 760);

    QTabWidget* tabs = new QTabWidget(this);

    // Вкладки разделяют две основные задачи программы:
    // редактирование справочника и расчет сроков закупки.
    tabs->addTab(buildDatabasePage(), "База");
    tabs->addTab(buildCalculationPage(), "Расчет");

    setCentralWidget(tabs);
}

QWidget* MainWindow::buildDatabasePage() {
    QWidget* page = new QWidget(this);

    itemsTree = new QTreeWidget(page);

    // База показывается деревом, чтобы было видно, какой материал к какой детали относится.
    // Например: Стул -> Ножка -> Брус.
    itemsTree->setColumnCount(6);
    itemsTree->setHeaderLabels({"Структура", "Тип", "Ед.", "Дни", "Изготовление параллельно / закуп одной партией", "Кол-во для родителя"});
    itemsTree->setSelectionBehavior(QAbstractItemView::SelectRows);
    itemsTree->setSelectionMode(QAbstractItemView::SingleSelection);
    itemsTree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    itemsTree->setAlternatingRowColors(true);
    itemsTree->header()->setSectionResizeMode(QHeaderView::Interactive);
    itemsTree->setColumnWidth(0, 420);
    itemsTree->setColumnWidth(4, 280);

    QPushButton* addButton = new QPushButton("Добавить", page);
    QPushButton* editButton = new QPushButton("Изменить", page);
    QPushButton* deleteButton = new QPushButton("Удалить", page);
    QPushButton* clearButton = new QPushButton("Удалить всю базу", page);
    QPushButton* saveButton = new QPushButton("Сохранить базу", page);

    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(addButton);
    buttonsLayout->addWidget(editButton);
    buttonsLayout->addWidget(deleteButton);
    buttonsLayout->addWidget(clearButton);
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(saveButton);

    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->addWidget(itemsTree);
    layout->addLayout(buttonsLayout);

    connect(addButton, &QPushButton::clicked, this, [this]() {
        addItem();
    });

    connect(editButton, &QPushButton::clicked, this, [this]() {
        editSelectedItem();
    });

    connect(deleteButton, &QPushButton::clicked, this, [this]() {
        deleteSelectedItem();
    });

    connect(clearButton, &QPushButton::clicked, this, [this]() {
        clearDatabase();
    });

    connect(saveButton, &QPushButton::clicked, this, [this]() {
        if (database.save()) {
            QMessageBox::information(this, "Готово", "База сохранена.");
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось сохранить базу.");
        }
    });

    return page;
}

QWidget* MainWindow::buildCalculationPage() {
    QWidget* page = new QWidget(this);

    productCombo = new QComboBox(page);
    // ComboBox оставлен редактируемым: можно выбрать изделие из базы или ввести название вручную.
    productCombo->setEditable(true);

    productQuantitySpin = new QDoubleSpinBox(page);
    productQuantitySpin->setRange(0.001, 1000000000);
    productQuantitySpin->setDecimals(3);
    productQuantitySpin->setSingleStep(1);

    requestDateEdit = new QDateEdit(QDate::currentDate(), page);
    requestDateEdit->setCalendarPopup(true);
    requestDateEdit->setDisplayFormat("dd.MM.yyyy");

    dueDateEdit = new QDateEdit(QDate::currentDate().addDays(30), page);
    dueDateEdit->setCalendarPopup(true);
    dueDateEdit->setDisplayFormat("dd.MM.yyyy");

    reserveSpin = new QSpinBox(page);
    reserveSpin->setRange(0, 3650);
    reserveSpin->setValue(10);

    QPushButton* calculateButton = new QPushButton("Рассчитать", page);

    QFormLayout* formLayout = new QFormLayout();
    formLayout->addRow("Готовый продукт:", productCombo);
    formLayout->addRow("Количество:", productQuantitySpin);
    formLayout->addRow("Дата обращения:", requestDateEdit);
    formLayout->addRow("Нужен к дате:", dueDateEdit);
    formLayout->addRow("Запас, дней:", reserveSpin);
    formLayout->addRow("", calculateButton);

    stockTable = new QTableWidget(page);
    // Верхняя таблица показывает, что должно лежать на складе к определенной дате.
    stockTable->setColumnCount(4);
    stockTable->setHorizontalHeaderLabels({"Дата склада", "Название", "Количество", "Ед."});
    stockTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    stockTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    stockTable->setSelectionMode(QAbstractItemView::SingleSelection);
    stockTable->setMinimumHeight(190);
    enableToggleSelection(stockTable);

    purchaseTable = new QTableWidget(page);
    // Нижняя таблица показывает сами закупки: когда заказать и сколько партий потребуется.
    purchaseTable->setColumnCount(8);
    purchaseTable->setHorizontalHeaderLabels({"Материал", "Количество", "Ед.", "На складе", "Крайний заказ", "Оптимальный заказ", "Поставка, дней", "Партий"});
    purchaseTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    purchaseTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    purchaseTable->setSelectionMode(QAbstractItemView::SingleSelection);
    purchaseTable->setMinimumHeight(190);
    enableToggleSelection(purchaseTable);

    summaryText = new QTextEdit(page);
    summaryText->setReadOnly(true);
    summaryText->setMinimumHeight(120);

    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->addLayout(formLayout);
    layout->addWidget(new QLabel("Что должно быть на складе:", page));
    layout->addWidget(stockTable);
    layout->addWidget(new QLabel("Сроки закупки:", page));
    layout->addWidget(purchaseTable);
    layout->addWidget(new QLabel("Итог:", page));
    layout->addWidget(summaryText);

    connect(calculateButton, &QPushButton::clicked, this, [this]() {
        calculatePlan();
    });

    return page;
}

bool MainWindow::itemIsUsedAsComponent(const string& name) const {
    // Помогает отличить готовые изделия верхнего уровня от деталей внутри других изделий.
    for (const Item& item : database.getItems()) {
        for (const Component& component : item.components) {
            if (component.name == name) {
                return true;
            }
        }
    }

    return false;
}

void MainWindow::addItemToTree(const Item& item, QTreeWidgetItem* parent, const QString& number, double quantity, const string& unit, vector<string>& path) {
    // Дерево показывает, из чего состоит изделие: изделие -> детали -> закупные материалы.
    QTreeWidgetItem* node = parent == nullptr ? new QTreeWidgetItem(itemsTree) : new QTreeWidgetItem(parent);
    QString title = number + ") " + QString::fromStdString(item.name);
    QString typeText = item.type == MANUFACTURED ? "Изготовление" : "Закупка";

    node->setText(0, title);
    node->setText(1, typeText);
    node->setText(2, QString::fromStdString(item.unit));
    node->setText(3, QString::number(item.days));
    node->setText(4, formatDouble(item.capacity));
    node->setText(5, parent == nullptr ? "-" : formatDouble(quantity) + " " + QString::fromStdString(unit));
    node->setData(0, Qt::UserRole, QString::fromStdString(item.name));

    if (std::find(path.begin(), path.end(), item.name) != path.end()) {
        QTreeWidgetItem* cycleNode = new QTreeWidgetItem(node);
        cycleNode->setText(0, number + ".! ) Повторная ссылка на " + QString::fromStdString(item.name));
        return;
    }

    path.push_back(item.name);

    for (int i = 0; i < static_cast<int>(item.components.size()); i++) {
        const Component& component = item.components[i];
        const Item* child = database.findItem(component.name);
        QString childNumber = number + "." + QString::number(i + 1);

        if (child == nullptr) {
            QTreeWidgetItem* missingNode = new QTreeWidgetItem(node);
            missingNode->setText(0, childNumber + ") " + QString::fromStdString(component.name) + " (нет в базе)");
            missingNode->setText(5, formatDouble(component.quantity) + " " + QString::fromStdString(component.unit));
            continue;
        }

        addItemToTree(*child, node, childNumber, component.quantity, component.unit, path);
    }

    path.pop_back();
}

void MainWindow::refreshItems() {
    itemsTree->clear();

    const vector<Item>& items = database.getItems();
    int rootNumber = 1;

    // Сначала показываем верхнеуровневые изделия, которые не являются компонентами других записей.
    for (const Item& item : items) {
        if (!itemIsUsedAsComponent(item.name)) {
            vector<string> path;
            addItemToTree(item, nullptr, QString::number(rootNumber), 1, item.unit, path);
            rootNumber++;
        }
    }

    // Если все записи зациклены или являются компонентами, все равно показываем их отдельными корнями.
    if (rootNumber == 1) {
        for (const Item& item : items) {
            vector<string> path;
            addItemToTree(item, nullptr, QString::number(rootNumber), 1, item.unit, path);
            rootNumber++;
        }
    }

    itemsTree->expandAll();
}

void MainWindow::refreshProductList() {
    QString currentText = productCombo->currentText();
    productCombo->clear();

    // Готовые изделия ставим выше деталей, чтобы случайно не считать ножку вместо стула.
    for (const Item& item : database.getItems()) {
        if (item.type == MANUFACTURED && !itemIsUsedAsComponent(item.name)) {
            productCombo->addItem(QString::fromStdString(item.name));
        }
    }

    for (const Item& item : database.getItems()) {
        if (item.type == MANUFACTURED && itemIsUsedAsComponent(item.name)) {
            productCombo->addItem(QString::fromStdString(item.name));
        }
    }

    if (!currentText.isEmpty()) {
        productCombo->setCurrentText(currentText);
    }
}

QString MainWindow::selectedItemName() const {
    QTreeWidgetItem* selected = itemsTree->currentItem();
    if (selected == nullptr) {
        return "";
    }

    return selected->data(0, Qt::UserRole).toString();
}

void MainWindow::addItem() {
    // Создание новой записи идет через тот же диалог, что и редактирование.
    ItemDialog dialog(&database, this);

    if (dialog.exec() == QDialog::Accepted) {
        database.addOrUpdateItem(dialog.getItem());
        database.save();
        refreshItems();
        refreshProductList();
    }
}

void MainWindow::editSelectedItem() {
    QString name = selectedItemName();
    if (name.isEmpty()) {
        QMessageBox::information(this, "Выбор", "Выберите запись для изменения.");
        return;
    }

    Item* item = database.findItem(name.toStdString());
    if (item == nullptr) {
        QMessageBox::warning(this, "Ошибка", "Запись не найдена.");
        return;
    }

    ItemDialog dialog(&database, this);
    // Передаем выбранную запись в диалог, чтобы пользователь видел текущие значения.
    dialog.setItem(*item);

    if (dialog.exec() == QDialog::Accepted) {
        Item updated = dialog.getItem();
        QString newName = QString::fromStdString(updated.name);

        if (newName != name) {
            if (database.findItem(updated.name) != nullptr) {
                QMessageBox::warning(this, "Ошибка", "Запись с новым названием уже существует.");
                return;
            }

            database.renameItem(name.toStdString(), updated.name);
        }

        database.addOrUpdateItem(updated);
        database.save();
        refreshItems();
        refreshProductList();
    }
}

void MainWindow::collectChildrenForDelete(const string& name, vector<string>& names) {
    if (std::find(names.begin(), names.end(), name) != names.end()) {
        return;
    }

    names.push_back(name);
    const Item* item = database.findItem(name);
    if (item == nullptr) {
        return;
    }

    // Удаление идет вместе со всеми вложенными компонентами выбранной записи.
    for (const Component& component : item->components) {
        collectChildrenForDelete(component.name, names);
    }
}

void MainWindow::deleteSelectedItem() {
    QString name = selectedItemName();
    if (name.isEmpty()) {
        QMessageBox::information(this, "Выбор", "Выберите запись для удаления.");
        return;
    }

    vector<string> namesToDelete;
    collectChildrenForDelete(name.toStdString(), namesToDelete);

    QString message = "Удалить запись \"" + name + "\"";
    if (namesToDelete.size() > 1) {
        message += " и все ее вложенные компоненты (" + QString::number(static_cast<int>(namesToDelete.size())) + " записей)";
    }
    message += "?";

    int answer = QMessageBox::question(this, "Удаление", message);

    if (answer == QMessageBox::Yes) {
        for (const string& itemName : namesToDelete) {
            database.removeItem(itemName);
        }

        database.save();
        refreshItems();
        refreshProductList();
    }
}

void MainWindow::clearDatabase() {
    int answer = QMessageBox::question(this, "Удаление всей базы", "Удалить все записи из базы?");

    if (answer == QMessageBox::Yes) {
        database.clear();
        database.save();
        refreshItems();
        refreshProductList();
        stockTable->setRowCount(0);
        purchaseTable->setRowCount(0);
        summaryText->clear();
    }
}

void MainWindow::calculatePlan() {
    QString productName = productCombo->currentText().trimmed();
    if (productName.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Выберите или введите готовый продукт.");
        return;
    }

    const Item* product = database.findItem(productName.toStdString());
    if (product == nullptr) {
        QMessageBox::warning(this, "Ошибка", "Готовый продукт не найден в базе.");
        return;
    }

    if (pieceUnit(QString::fromStdString(product->unit)) && !wholeNumber(productQuantitySpin->value())) {
        QMessageBox::warning(this, "Ошибка", "Для единицы измерения \"шт\" количество готового продукта должно быть целым.");
        return;
    }

    Date requestDate = fromQDate(requestDateEdit->date());
    Date dueDate = fromQDate(dueDateEdit->date());

    Planner planner(database);
    // Planner считает "сырой" план: складские потребности, закупки и ошибки.
    // Окно уже превращает эти данные в таблицы, понятные пользователю.
    PlanResult result = planner.makePlan(productName.toStdString(), productQuantitySpin->value(), dueDate, reserveSpin->value());

    if (!result.errors.empty()) {
        QString text;
        for (const string& error : result.errors) {
            text += "- " + QString::fromStdString(error) + "\n";
        }
        QMessageBox::warning(this, "Ошибки расчета", text);
    }

    // Для вывода объединяем одинаковые материалы, но не теряем разные даты склада.
    vector<StockNeed> stockNeeds = aggregateStockNeedsForReport(result.stockNeeds);
    vector<PurchaseNeed> purchases = aggregatePurchasesForReport(result.purchases, result.reserveDays);
    int delayDays = calculateDelayDays(purchases, requestDate);

    // Если крайний срок закупки уже прошел, показываем таблицы по лучшему возможному графику от даты обращения.
    fillStockTable(shiftStockDates(stockNeeds, delayDays));
    fillPurchaseTable(shiftPurchaseDates(purchases, delayDays));
    summaryText->setHtml(buildSummary(result, purchases, requestDate));
}

void MainWindow::fillStockTable(const vector<StockNeed>& stockNeeds) {
    stockTable->setRowCount(static_cast<int>(stockNeeds.size()));

    for (int row = 0; row < static_cast<int>(stockNeeds.size()); row++) {
        const StockNeed& need = stockNeeds[row];
        stockTable->setItem(row, 0, new QTableWidgetItem(formatDateQt(need.readyDate)));
        stockTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(need.name)));
        stockTable->setItem(row, 2, new QTableWidgetItem(formatDouble(need.quantity)));
        stockTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(need.unit)));
    }

    stockTable->resizeRowsToContents();
}

void MainWindow::fillPurchaseTable(const vector<PurchaseNeed>& purchases) {
    purchaseTable->setRowCount(static_cast<int>(purchases.size()));

    for (int row = 0; row < static_cast<int>(purchases.size()); row++) {
        const PurchaseNeed& purchase = purchases[row];
        purchaseTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(purchase.name)));
        purchaseTable->setItem(row, 1, new QTableWidgetItem(formatDouble(purchase.quantity)));
        purchaseTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(purchase.unit)));
        purchaseTable->setItem(row, 3, new QTableWidgetItem(formatDateQt(purchase.needDate)));
        purchaseTable->setItem(row, 4, new QTableWidgetItem(formatDateQt(purchase.deadlineDate)));
        purchaseTable->setItem(row, 5, new QTableWidgetItem(formatDateQt(purchase.optimalDate)));
        purchaseTable->setItem(row, 6, new QTableWidgetItem(QString::number(purchase.deliveryDays)));
        purchaseTable->setItem(row, 7, new QTableWidgetItem(QString::number(purchase.batches)));
    }

    purchaseTable->resizeRowsToContents();
}

int MainWindow::calculateDelayDays(const vector<PurchaseNeed>& purchases, const Date& requestDate) const {
    // Если хотя бы один крайний срок закупки уже прошел, ищем максимальное опоздание.
    // Потом на это количество дней сдвигаем таблицы, чтобы показать лучший реальный график от даты обращения.
    int maxDelayDays = 0;

    for (const PurchaseNeed& purchase : purchases) {
        if (isDateBefore(purchase.deadlineDate, requestDate)) {
            int delayDays = daysBetween(purchase.deadlineDate, requestDate);
            if (delayDays > maxDelayDays) {
                maxDelayDays = delayDays;
            }
        }
    }

    return maxDelayDays;
}

vector<StockNeed> MainWindow::shiftStockDates(const vector<StockNeed>& stockNeeds, int delayDays) const {
    vector<StockNeed> result = stockNeeds;

    if (delayDays <= 0) {
        return result;
    }

    for (StockNeed& need : result) {
        need.readyDate = addDays(need.readyDate, delayDays);
    }

    return result;
}

vector<PurchaseNeed> MainWindow::shiftPurchaseDates(const vector<PurchaseNeed>& purchases, int delayDays) const {
    vector<PurchaseNeed> result = purchases;

    if (delayDays <= 0) {
        return result;
    }

    for (PurchaseNeed& purchase : result) {
        purchase.needDate = addDays(purchase.needDate, delayDays);
        purchase.deadlineDate = addDays(purchase.deadlineDate, delayDays);
        purchase.optimalDate = addDays(purchase.optimalDate, delayDays);
    }

    return result;
}

QString MainWindow::buildSummary(const PlanResult& result, const vector<PurchaseNeed>& purchases, const Date& requestDate) const {
    // Итог выводится HTML-текстом, чтобы можно было выделить важные даты жирным.
    QString text = "<div style='font-size:15px; line-height:1.35;'>";

    if (!result.errors.empty()) {
        text += "Расчет содержит ошибки. Проверьте записи в базе.</div>";
        return text;
    }

    if (purchases.empty()) {
        return text + "Закупаемых материалов в составе нет.</div>";
    }

    Date minOptimal = purchases[0].optimalDate;
    Date minDeadline = purchases[0].deadlineDate;
    int maxDelayDays = 0;

    for (const PurchaseNeed& purchase : purchases) {
        if (isDateBefore(purchase.optimalDate, minOptimal)) {
            minOptimal = purchase.optimalDate;
        }
        if (isDateBefore(purchase.deadlineDate, minDeadline)) {
            minDeadline = purchase.deadlineDate;
        }
        if (isDateBefore(purchase.deadlineDate, requestDate)) {
            int delayDays = daysBetween(purchase.deadlineDate, requestDate);
            if (delayDays > maxDelayDays) {
                maxDelayDays = delayDays;
            }
        }
    }

    if (maxDelayDays > 0) {
        Date bestFinishDate = addDays(result.dueDate, maxDelayDays);
        Date finishDateWithReserve = addDays(bestFinishDate, result.reserveDays);

        text += "<b>Внимание:</b> сроки не выполняются.<br>";
        text += "Если заказать товары прямо сейчас, изделие будет готово в лучшем случае к <b>"
              + formatDateQt(bestFinishDate) + "</b> без учета задержек.<br>";
        text += "Самая поздняя дата готовности с учетом запаса "
              + QString::number(result.reserveDays) + " дней: <b>"
              + formatDateQt(finishDateWithReserve) + "</b><br>";
    } else {
        text += "Самый ранний оптимальный срок закупки: <b>" + formatDateQt(minOptimal) + "</b><br>";
        text += "Самый ранний крайний срок закупки: <b>" + formatDateQt(minDeadline) + "</b><br>";
    }

    text += "</div>";
    return text;
}
