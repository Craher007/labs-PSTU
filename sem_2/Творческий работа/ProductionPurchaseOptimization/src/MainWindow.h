#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "Database.h"
#include "Planner.h"
#include <QComboBox>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QMainWindow>
#include <QSpinBox>
#include <QTableWidget>
#include <QTextEdit>
#include <QTreeWidget>

using namespace std;

class MainWindow : public QMainWindow {
private:
    // Главное окно владеет базой и передает ее в диалоги/планировщик.
    Database database;

    // Виджеты вкладки "База".
    QTreeWidget* itemsTree;

    // Виджеты вкладки "Расчет".
    QComboBox* productCombo;
    QDoubleSpinBox* productQuantitySpin;
    QDateEdit* requestDateEdit;
    QDateEdit* dueDateEdit;
    QSpinBox* reserveSpin;
    QTableWidget* stockTable;
    QTableWidget* purchaseTable;
    QTextEdit* summaryText;

    // Небольшие вспомогательные функции форматирования и проверки ввода.
    QString formatDouble(double value) const;
    QString formatDateQt(const Date& date) const;
    Date fromQDate(const QDate& date) const;
    bool pieceUnit(const QString& unit) const;
    bool wholeNumber(double value) const;

    void buildUi();
    QWidget* buildDatabasePage();
    QWidget* buildCalculationPage();

    // Методы вкладки "База": обновление дерева, создание, изменение и удаление записей.
    void refreshItems();
    void refreshProductList();
    void addItem();
    void editSelectedItem();
    void deleteSelectedItem();
    void clearDatabase();

    // Методы вкладки "Расчет": запуск планировщика и заполнение таблиц результата.
    void calculatePlan();
    void fillStockTable(const vector<StockNeed>& stockNeeds);
    void fillPurchaseTable(const vector<PurchaseNeed>& purchases);
    QString buildSummary(const PlanResult& result, const vector<PurchaseNeed>& purchases, const Date& requestDate) const;
    int calculateDelayDays(const vector<PurchaseNeed>& purchases, const Date& requestDate) const;
    vector<StockNeed> shiftStockDates(const vector<StockNeed>& stockNeeds, int delayDays) const;
    vector<PurchaseNeed> shiftPurchaseDates(const vector<PurchaseNeed>& purchases, int delayDays) const;

    // Методы для работы с деревом базы и зависимыми компонентами.
    QString selectedItemName() const;
    bool itemIsUsedAsComponent(const string& name) const;
    void addItemToTree(const Item& item, QTreeWidgetItem* parent, const QString& number, double quantity, const string& unit, vector<string>& path);
    void collectChildrenForDelete(const string& name, vector<string>& names);

public:
    MainWindow(QWidget* parent = nullptr);
};

#endif
