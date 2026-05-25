#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QString>
#include <QVector>
#include <QWidget>

class GraphCanvas;
class QSpinBox;
class QTextEdit;

class MainWindow : public QWidget {
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    GraphCanvas* canvas = nullptr;
    QSpinBox* weightSpin = nullptr;
    QTextEdit* logEdit = nullptr;

    void solve();
    QString pathToString(const QVector<int>& path) const;
};

#endif
