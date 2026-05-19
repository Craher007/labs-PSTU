#include "ItemDialog.h"
#include <QDialogButtonBox>
#include <QFont>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <algorithm>
#include <cmath>

using namespace std;

ItemDialog::ItemDialog(Database* database, QWidget* parent) : QDialog(parent) {
    this->database = database;
    setWindowTitle("Запись");
    resize(1120, 740);
    setMinimumSize(980, 650);

    nameEdit = new QLineEdit(this);
    unitEdit = new QLineEdit(this);

    typeCombo = new QComboBox(this);
    typeCombo->addItem("Изготавливается", MANUFACTURED);
    typeCombo->addItem("Закупается", PURCHASED);

    daysSpin = new QSpinBox(this);
    daysSpin->setRange(0, 100000);

    capacitySpin = new QDoubleSpinBox(this);
    capacitySpin->setRange(0, 1000000000);
    capacitySpin->setDecimals(3);
    capacitySpin->setSingleStep(1);

    QGroupBox* mainGroup = new QGroupBox("Основные данные", this);
    // Верхняя часть диалога описывает саму запись: название, тип и сроки.
    QFormLayout* formLayout = new QFormLayout(mainGroup);
    formLayout->addRow("Название:", nameEdit);
    formLayout->addRow("Единица измерения:", unitEdit);
    formLayout->addRow("Тип:", typeCombo);
    formLayout->addRow("Дней на цикл/поставку:", daysSpin);
    formLayout->addRow("Изготовление параллельно / закуп одной партией:", capacitySpin);

    componentsTree = new QTreeWidget(this);
    componentsTree->setObjectName("componentsTree");
    // Состав изделия сделан деревом, чтобы прямо в окне записи видеть вложенность:
    // изделие -> деталь -> закупаемый материал.
    componentsTree->setColumnCount(7);
    componentsTree->setHeaderLabels({"№", "Название", "Ед.", "Тип", "Дней", "Изготовление параллельно / закуп одной партией", "Количество на 1 изделие"});
    componentsTree->setSelectionBehavior(QAbstractItemView::SelectRows);
    componentsTree->setSelectionMode(QAbstractItemView::SingleSelection);
    componentsTree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    componentsTree->header()->setSectionResizeMode(QHeaderView::Interactive);
    componentsTree->setColumnWidth(0, 90);
    componentsTree->setColumnWidth(1, 240);
    componentsTree->setColumnWidth(2, 90);
    componentsTree->setColumnWidth(3, 150);
    componentsTree->setColumnWidth(4, 90);
    componentsTree->setColumnWidth(5, 320);
    componentsTree->setColumnWidth(6, 170);
    componentsTree->setMinimumHeight(340);
    componentsTree->setMouseTracking(true);

    QPushButton* addComponentButton = new QPushButton("Добавить компонент", this);
    QPushButton* removeComponentButton = new QPushButton("Удалить компонент", this);
    QPushButton* editComponentButton = new QPushButton("Создать/изменить выбранный компонент", this);

    QHBoxLayout* componentButtonsLayout = new QHBoxLayout();
    componentButtonsLayout->addWidget(addComponentButton);
    componentButtonsLayout->addWidget(removeComponentButton);
    componentButtonsLayout->addWidget(editComponentButton);
    componentButtonsLayout->addStretch();

    QGroupBox* componentsGroup = new QGroupBox("Состав изделия", this);
    QVBoxLayout* componentsLayout = new QVBoxLayout(componentsGroup);
    componentsLayout->addWidget(componentsTree);
    componentsLayout->addLayout(componentButtonsLayout);

    QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttons->button(QDialogButtonBox::Ok)->setText("Сохранить");
    buttons->button(QDialogButtonBox::Cancel)->setText("Отмена");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(mainGroup);
    mainLayout->addWidget(componentsGroup);
    mainLayout->addWidget(buttons);

    connect(typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]() {
        updateTypeView();
    });

    connect(addComponentButton, &QPushButton::clicked, this, [this]() {
        addComponentRow();
    });

    connect(removeComponentButton, &QPushButton::clicked, this, [this]() {
        QTreeWidgetItem* item = selectedComponentItem();
        if (item != nullptr && item->parent() == nullptr) {
            delete item;
            refreshComponentNumbers();
        } else {
            QMessageBox::information(this, "Выбор", "Удалять можно только компоненты первого уровня текущей записи.");
        }
    });

    connect(editComponentButton, &QPushButton::clicked, this, [this]() {
        editSelectedComponent();
    });

    connect(buttons, &QDialogButtonBox::accepted, this, [this]() {
        if (validateItem()) {
            saveInlinePurchasedComponents();
            accept();
        }
    });

    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    updateTypeView();
}

bool ItemDialog::pieceUnit(const QString& unit) const {
    QString value = cleanText(unit).toLower();
    return value == "шт" || value == "штук" || value == "штука" || value == "штуки";
}

bool ItemDialog::wholeNumber(double value) const {
    return fabs(value - round(value)) < 0.0001;
}

QString ItemDialog::cleanText(const QString& value) const {
    return value.trimmed();
}

void ItemDialog::updateTypeView() {
    ItemType type = static_cast<ItemType>(typeCombo->currentData().toInt());
    // Закупаемый материал не имеет состава, поэтому дерево компонентов для него отключаем.
    componentsTree->setEnabled(type == MANUFACTURED);
}

QLineEdit* ItemDialog::createCellEdit(const QString& value) {
    QLineEdit* edit = new QLineEdit(componentsTree);
    edit->setText(value);
    edit->setFrame(false);
    edit->setContentsMargins(4, 0, 4, 0);
    edit->setStyleSheet(
        "QLineEdit { background-color: transparent; color: #111111; padding: 2px 4px; }"
        "QLineEdit:hover { background-color: #f7f7f7; }"
        "QLineEdit:focus { background-color: #f2f2f2; border: 1px solid #c8c8c8; }"
    );
    return edit;
}

static void applyNestedItemStyle(QTreeWidgetItem* item) {
    if (item == nullptr) {
        return;
    }

    QFont font;
    font.setPointSize(9);
    font.setBold(false);

    for (int column = 0; column < item->columnCount(); column++) {
        item->setFont(column, font);
        item->setForeground(column, QBrush(QColor("#111111")));
        item->setBackground(column, QBrush(QColor("#f9f9f9")));
    }
}

QComboBox* ItemDialog::createComponentTypeCombo(ItemType type) {
    QComboBox* combo = new QComboBox(componentsTree);
    combo->addItem("Изготавливается", MANUFACTURED);
    combo->addItem("Закупается", PURCHASED);

    int index = combo->findData(type);
    if (index >= 0) {
        combo->setCurrentIndex(index);
    }

    return combo;
}

QSpinBox* ItemDialog::createDaysSpin(int days) {
    QSpinBox* spin = new QSpinBox(componentsTree);
    spin->setRange(0, 100000);
    spin->setValue(days);
    return spin;
}

QDoubleSpinBox* ItemDialog::createCapacitySpin(double capacity) {
    QDoubleSpinBox* spin = new QDoubleSpinBox(componentsTree);
    spin->setRange(0, 1000000000);
    spin->setDecimals(3);
    spin->setSingleStep(1);
    spin->setValue(capacity);
    return spin;
}

void ItemDialog::addNestedComponents(QTreeWidgetItem* parent, const Item& item, const QString& number, vector<string>& path) {
    // Рекурсивно добавляем вложенные компоненты только для просмотра.
    // Редактирование вложенной записи делается через кнопку "Создать/изменить выбранный компонент".
    if (std::find(path.begin(), path.end(), item.name) != path.end()) {
        QTreeWidgetItem* cycleItem = new QTreeWidgetItem(parent);
        cycleItem->setText(0, number + ".!");
        cycleItem->setText(1, "Повторная ссылка на " + QString::fromStdString(item.name));
        return;
    }

    path.push_back(item.name);

    for (int i = 0; i < static_cast<int>(item.components.size()); i++) {
        const Component& component = item.components[i];
        const Item* child = database == nullptr ? nullptr : database->findItem(component.name);
        QString childNumber = number + "." + QString::number(i + 1);

        QTreeWidgetItem* childItem = new QTreeWidgetItem(parent);
        childItem->setText(0, childNumber);
        childItem->setText(1, QString::fromStdString(component.name));
        childItem->setText(2, QString::fromStdString(component.unit));
        childItem->setText(6, QString::number(component.quantity, 'f', 3));
        childItem->setData(0, Qt::UserRole, false);
        applyNestedItemStyle(childItem);

        if (child != nullptr) {
            childItem->setText(3, child->type == MANUFACTURED ? "Изготовление" : "Закупка");
            childItem->setText(4, QString::number(child->days));
            childItem->setText(5, QString::number(child->capacity, 'f', 3));
            addNestedComponents(childItem, *child, childNumber, path);
        } else {
            childItem->setText(3, "Нет в базе");
        }
    }

    path.pop_back();
}

void ItemDialog::refreshComponentNumbers() {
    // После добавления/удаления пересобираем номера 1, 1.1, 1.1.1 и заново подтягиваем вложенные компоненты из базы.
    for (int i = 0; i < componentsTree->topLevelItemCount(); i++) {
        QTreeWidgetItem* item = componentsTree->topLevelItem(i);
        QString number = QString::number(i + 1);
        item->setText(0, number);

        while (item->childCount() > 0) {
            delete item->takeChild(0);
        }

        Component component = getComponentFromItem(item);
        const Item* existing = database == nullptr ? nullptr : database->findItem(component.name);
        if (existing != nullptr) {
            vector<string> path;
            addNestedComponents(item, *existing, number, path);
        }
    }

    componentsTree->expandAll();
}

void ItemDialog::addComponentRow(const Component& component) {
    // Строка первого уровня - это компонент текущего изделия.
    // В ней можно сразу указать тип, сроки, партию и количество на одну единицу изделия.
    QTreeWidgetItem* item = new QTreeWidgetItem(componentsTree);
    item->setData(0, Qt::UserRole, true);
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);

    // Если компонент уже есть в базе, подставляем его тип, сроки и лимит партии.
    ItemType componentType = MANUFACTURED;
    int componentDays = 1;
    double componentCapacity = 1;
    if (database != nullptr) {
        const Item* existing = database->findItem(component.name);
        if (existing != nullptr) {
            componentType = existing->type;
            componentDays = existing->days;
            componentCapacity = existing->capacity;
        }
    }

    componentsTree->setItemWidget(item, 1, createCellEdit(QString::fromStdString(component.name)));
    componentsTree->setItemWidget(item, 2, createCellEdit(QString::fromStdString(component.unit)));
    componentsTree->setItemWidget(item, 3, createComponentTypeCombo(componentType));
    componentsTree->setItemWidget(item, 4, createDaysSpin(componentDays));
    componentsTree->setItemWidget(item, 5, createCapacitySpin(componentCapacity));

    QDoubleSpinBox* quantitySpin = new QDoubleSpinBox(componentsTree);
    quantitySpin->setRange(0.001, 1000000000);
    quantitySpin->setDecimals(3);
    quantitySpin->setSingleStep(1);
    quantitySpin->setValue(component.quantity > 0 ? component.quantity : 1);
    componentsTree->setItemWidget(item, 6, quantitySpin);

    refreshComponentNumbers();
}

QTreeWidgetItem* ItemDialog::selectedComponentItem() const {
    return componentsTree->currentItem();
}

Component ItemDialog::getComponentFromItem(QTreeWidgetItem* item) const {
    Component component;
    if (item == nullptr) {
        return component;
    }

    QDoubleSpinBox* quantitySpin = qobject_cast<QDoubleSpinBox*>(componentsTree->itemWidget(item, 6));
    QLineEdit* nameWidget = qobject_cast<QLineEdit*>(componentsTree->itemWidget(item, 1));
    QLineEdit* unitWidget = qobject_cast<QLineEdit*>(componentsTree->itemWidget(item, 2));

    component.name = cleanText(nameWidget == nullptr ? item->text(1) : nameWidget->text()).toStdString();
    component.unit = cleanText(unitWidget == nullptr ? item->text(2) : unitWidget->text()).toStdString();
    component.quantity = quantitySpin == nullptr ? item->text(6).toDouble() : quantitySpin->value();
    return component;
}

ItemType ItemDialog::getComponentTypeFromItem(QTreeWidgetItem* item) const {
    QComboBox* combo = qobject_cast<QComboBox*>(componentsTree->itemWidget(item, 3));

    if (combo == nullptr) {
        return item != nullptr && item->text(3) == "Закупка" ? PURCHASED : MANUFACTURED;
    }

    return static_cast<ItemType>(combo->currentData().toInt());
}

int ItemDialog::getComponentDaysFromItem(QTreeWidgetItem* item) const {
    QSpinBox* spin = qobject_cast<QSpinBox*>(componentsTree->itemWidget(item, 4));

    if (spin == nullptr) {
        return item == nullptr ? 1 : item->text(4).toInt();
    }

    return spin->value();
}

double ItemDialog::getComponentCapacityFromItem(QTreeWidgetItem* item) const {
    QDoubleSpinBox* spin = qobject_cast<QDoubleSpinBox*>(componentsTree->itemWidget(item, 5));

    if (spin == nullptr) {
        return item == nullptr ? 0 : item->text(5).toDouble();
    }

    return spin->value();
}

void ItemDialog::saveInlinePurchasedComponents() {
    if (database == nullptr) {
        return;
    }

    // Закупные компоненты можно заполнить прямо в строке состава.
    // Здесь они автоматически сохраняются как отдельные записи базы.
    for (int i = 0; i < componentsTree->topLevelItemCount(); i++) {
        QTreeWidgetItem* item = componentsTree->topLevelItem(i);
        if (getComponentTypeFromItem(item) != PURCHASED) {
            continue;
        }

        Component component = getComponentFromItem(item);
        if (component.name.empty() || component.unit.empty()) {
            continue;
        }

        Item purchased;
        purchased.name = component.name;
        purchased.unit = component.unit;
        purchased.type = PURCHASED;
        purchased.days = getComponentDaysFromItem(item);
        purchased.capacity = getComponentCapacityFromItem(item);

        database->addOrUpdateItem(purchased);
    }

    database->save();
}

void ItemDialog::editSelectedComponent() {
    if (database == nullptr) {
        QMessageBox::warning(this, "Ошибка", "База данных недоступна.");
        return;
    }

    QTreeWidgetItem* selected = selectedComponentItem();
    if (selected == nullptr) {
        QMessageBox::information(this, "Выбор", "Выберите компонент в дереве состава.");
        return;
    }

    Component component = getComponentFromItem(selected);
    QString componentName = QString::fromStdString(component.name);
    QString componentUnit = QString::fromStdString(component.unit);

    if (componentName.trimmed().isEmpty() || componentUnit.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Сначала заполните название и единицу измерения компонента.");
        return;
    }

    if (componentName == cleanText(nameEdit->text())) {
        QMessageBox::warning(this, "Ошибка", "Компонент не должен ссылаться сам на себя.");
        return;
    }

    Item child;
    const Item* existing = database->findItem(component.name);
    ItemType selectedType = getComponentTypeFromItem(selected);

    // Если запись уже есть в базе, берем ее состав, но обновляем тип/сроки тем, что пользователь сейчас указал в родительском окне.
    if (existing != nullptr) {
        child = *existing;
        child.type = selectedType;
        child.days = getComponentDaysFromItem(selected);
        child.capacity = getComponentCapacityFromItem(selected);
    } else {
        child.name = component.name;
        child.unit = component.unit;
        child.type = selectedType;
        child.days = getComponentDaysFromItem(selected);
        child.capacity = getComponentCapacityFromItem(selected);
    }

    ItemDialog dialog(database, this);
    dialog.setItem(child);

    if (dialog.exec() == QDialog::Accepted) {
        Item updated = dialog.getItem();
        database->addOrUpdateItem(updated);
        database->save();

        QLineEdit* nameWidget = qobject_cast<QLineEdit*>(componentsTree->itemWidget(selected, 1));
        QLineEdit* unitWidget = qobject_cast<QLineEdit*>(componentsTree->itemWidget(selected, 2));

        if (nameWidget != nullptr) {
            nameWidget->setText(QString::fromStdString(updated.name));
        } else {
            selected->setText(1, QString::fromStdString(updated.name));
        }

        if (unitWidget != nullptr) {
            unitWidget->setText(QString::fromStdString(updated.unit));
        } else {
            selected->setText(2, QString::fromStdString(updated.unit));
        }

        QComboBox* typeWidget = qobject_cast<QComboBox*>(componentsTree->itemWidget(selected, 3));
        QSpinBox* daysWidget = qobject_cast<QSpinBox*>(componentsTree->itemWidget(selected, 4));
        QDoubleSpinBox* capacityWidget = qobject_cast<QDoubleSpinBox*>(componentsTree->itemWidget(selected, 5));

        if (typeWidget != nullptr) {
            typeWidget->setCurrentIndex(typeWidget->findData(updated.type));
        } else {
            selected->setText(3, updated.type == MANUFACTURED ? "Изготовление" : "Закупка");
        }

        if (daysWidget != nullptr) {
            daysWidget->setValue(updated.days);
        } else {
            selected->setText(4, QString::number(updated.days));
        }

        if (capacityWidget != nullptr) {
            capacityWidget->setValue(updated.capacity);
        } else {
            selected->setText(5, QString::number(updated.capacity, 'f', 3));
        }

        refreshComponentNumbers();
        QMessageBox::information(this, "Готово", "Компонент сохранен в базе. Теперь его можно использовать в расчете.");
    }
}

bool ItemDialog::validateItem() {
    // Перед сохранением проверяем данные, чтобы в базу не попадали пустые компоненты и дробные штуки там, где пользователь указал единицу измерения "шт".
    QString name = cleanText(nameEdit->text());
    QString unit = cleanText(unitEdit->text());

    if (name.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите название записи.");
        return false;
    }

    if (unit.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите единицу измерения.");
        return false;
    }

    if (pieceUnit(unit) && !wholeNumber(capacitySpin->value())) {
        QMessageBox::warning(this, "Ошибка", "Для единицы измерения \"шт\" значение параллельного изготовления/партии должно быть целым числом.");
        return false;
    }

    ItemType type = static_cast<ItemType>(typeCombo->currentData().toInt());
    if (type == MANUFACTURED && capacitySpin->value() <= 0) {
        QMessageBox::warning(this, "Ошибка", "Для изготовления значение параллельного изготовления должно быть больше 0.");
        return false;
    }

    if (type == MANUFACTURED) {
        for (int i = 0; i < componentsTree->topLevelItemCount(); i++) {
            QTreeWidgetItem* item = componentsTree->topLevelItem(i);
            Component component = getComponentFromItem(item);
            QString componentName = QString::fromStdString(component.name);
            QString componentUnit = QString::fromStdString(component.unit);
            double capacity = getComponentCapacityFromItem(item);
            ItemType componentType = getComponentTypeFromItem(item);

            if (componentName.isEmpty() || componentUnit.isEmpty()) {
                QMessageBox::warning(this, "Ошибка", "У компонента не заполнено название или единица измерения.");
                return false;
            }

            if (componentName == name) {
                QMessageBox::warning(this, "Ошибка", "Компонент не должен совпадать с названием текущей записи.");
                return false;
            }

            if (pieceUnit(componentUnit) && !wholeNumber(component.quantity)) {
                QMessageBox::warning(this, "Ошибка", "Для компонента в штуках количество должно быть целым числом.");
                return false;
            }

            if (pieceUnit(componentUnit) && capacity > 0 && !wholeNumber(capacity)) {
                QMessageBox::warning(this, "Ошибка", "Для компонента в штуках параллельное изготовление/партия закупки должны быть целым числом.");
                return false;
            }

            if (componentType == PURCHASED && getComponentDaysFromItem(item) <= 0) {
                QMessageBox::warning(this, "Ошибка", "Для закупаемого компонента срок поставки должен быть больше 0.");
                return false;
            }
        }
    }

    return true;
}

void ItemDialog::setItem(const Item& item) {
    // Заполняем диалог существующей записью. Этим же методом пользуется вложенный редактор.
    nameEdit->setText(QString::fromStdString(item.name));
    unitEdit->setText(QString::fromStdString(item.unit));

    int typeIndex = typeCombo->findData(item.type);
    if (typeIndex >= 0) {
        typeCombo->setCurrentIndex(typeIndex);
    }

    daysSpin->setValue(item.days);
    capacitySpin->setValue(item.capacity);

    componentsTree->clear();
    for (const Component& component : item.components) {
        addComponentRow(component);
    }

    refreshComponentNumbers();
    updateTypeView();
}

Item ItemDialog::getItem() const {
    // Собираем объект Item из виджетов. В базу сохраняются только компоненты первого уровня: вложенные компоненты принадлежат своим отдельным записям.
    Item item;
    item.name = cleanText(nameEdit->text()).toStdString();
    item.unit = cleanText(unitEdit->text()).toStdString();
    item.type = static_cast<ItemType>(typeCombo->currentData().toInt());
    item.days = daysSpin->value();
    item.capacity = capacitySpin->value();

    if (item.type == MANUFACTURED) {
        for (int i = 0; i < componentsTree->topLevelItemCount(); i++) {
            Component component = getComponentFromItem(componentsTree->topLevelItem(i));

            if (!component.name.empty() && component.quantity > 0) {
                item.components.push_back(component);
            }
        }
    }

    return item;
}
