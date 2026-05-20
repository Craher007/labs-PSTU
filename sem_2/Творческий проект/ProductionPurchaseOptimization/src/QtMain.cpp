#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "MainWindow.h"
#include <QApplication>
#include <QDir>

using namespace std;

int main(int argc, char* argv[]) {
#ifdef _WIN32
    // На Windows включаем UTF-8 для корректной работы русских строк.
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    QApplication app(argc, argv);

    // Общий стиль приложения вынесен сюда, чтобы все окна выглядели одинаково.
    // Отдельные правила для таблиц и дерева состава нужны, чтобы выделение не мешало чтению данных.
    app.setStyleSheet(
        "QWidget { background-color: #eeeeee; }"
        "QLineEdit, QComboBox, QSpinBox, QDoubleSpinBox, QDateEdit, QTextEdit, "
        "QTableWidget, QTreeWidget { background-color: #f9f9f9; color: #111111; }"
        "QTableWidget::item:selected { background-color: #eeeeee; color: #111111; }"
        "QTableWidget::item:selected:active { background-color: #e6e6e6; color: #111111; }"
        "QTableWidget::item:selected:!active { background-color: #f2f2f2; color: #111111; }"
        "QTreeWidget::item:selected { background-color: #555555; color: white; }"
        "QTreeWidget#componentsTree::item:hover { background-color: #f4f4f4; color: #111111; }"
        "QTreeWidget#componentsTree::item:selected { background-color: #eeeeee; color: #111111; }"
        "QTreeWidget#componentsTree::item:selected:active { background-color: #e8e8e8; color: #111111; }"
        "QTreeWidget#componentsTree::item:selected:!active { background-color: #f2f2f2; color: #111111; }"
        "QHeaderView::section { background-color: #dddddd; padding: 5px; border: 1px solid #c7c7c7; }"
        "QPushButton { background-color: #e1e1e1; padding: 6px 12px; border: 1px solid #b8b8b8; border-radius: 3px; }"
        "QPushButton:hover { background-color: #d4d4d4; }"
        "QPushButton:pressed { background-color: #555555; color: white; }"
        "QGroupBox { border: 1px solid #c6c6c6; margin-top: 10px; padding: 8px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 8px; padding: 0 4px; }"
        "QCalendarWidget QWidget { background-color: #f5f5f5; color: #111111; }"
        "QCalendarWidget QToolButton { background-color: #dcdcdc; color: #111111; font-weight: bold; }"
        "QCalendarWidget QMenu { background-color: #f5f5f5; color: #111111; }"
        "QCalendarWidget QSpinBox { background-color: #f5f5f5; color: #111111; }"
        "QCalendarWidget QAbstractItemView { background-color: #f9f9f9; color: #111111; selection-background-color: #555555; selection-color: white; }"
    );

    QDir::setCurrent(QCoreApplication::applicationDirPath());

    // Главное окно содержит две вкладки: "База" и "Расчет".
    MainWindow window;
    window.showMaximized();
    window.raise();
    window.activateWindow();

    return app.exec();
}
