#ifndef ITEM_DIALOG_H
#define ITEM_DIALOG_H

#include "Database.h"
#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QTreeWidget>

using namespace std;

class ItemDialog : public QDialog {
private:
    // Диалог работает с общей базой, чтобы можно было сразу создать/изменить вложенный компонент.
    Database* database;

    // Основные поля записи: изделие, деталь или закупаемый материал.
    QLineEdit* nameEdit;
    QLineEdit* unitEdit;
    QComboBox* typeCombo;
    QSpinBox* daysSpin;
    QDoubleSpinBox* capacitySpin;

    // Дерево состава показывает компоненты первого уровня и их вложенные материалы.
    QTreeWidget* componentsTree;

    // Проверки и небольшие преобразования пользовательского ввода.
    bool pieceUnit(const QString& unit) const;
    bool wholeNumber(double value) const;
    QString cleanText(const QString& value) const;
    void updateTypeView();

    // Работа со строками дерева состава изделия.
    void addComponentRow(const Component& component = Component());
    void refreshComponentNumbers();
    void addNestedComponents(QTreeWidgetItem* parent, const Item& item, const QString& number, vector<string>& path);
    QTreeWidgetItem* selectedComponentItem() const;
    Component getComponentFromItem(QTreeWidgetItem* item) const;
    ItemType getComponentTypeFromItem(QTreeWidgetItem* item) const;
    int getComponentDaysFromItem(QTreeWidgetItem* item) const;
    double getComponentCapacityFromItem(QTreeWidgetItem* item) const;

    // Фабрики виджетов для ячеек дерева.
    QLineEdit* createCellEdit(const QString& value);
    QComboBox* createComponentTypeCombo(ItemType type);
    QSpinBox* createDaysSpin(int days);
    QDoubleSpinBox* createCapacitySpin(double capacity);
    void saveInlinePurchasedComponents();
    void editSelectedComponent();
    bool validateItem();

public:
    ItemDialog(Database* database = nullptr, QWidget* parent = nullptr);

    void setItem(const Item& item);
    Item getItem() const;
};

#endif
