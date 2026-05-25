#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Graph.h"

#include <QMainWindow>

class GraphView;
class QLineEdit;
class QTableWidget;
class QTextEdit;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void addNode();
    void removeNode();
    void createArc();
    void removeArc();
    void changeArc();
    void runWidthSearch();
    void runDepthSearch();
    void runDijkstra();
    void runFloyd();
    void editMatrix(int row, int column);
    void resetMap();

private:
    WeightedGraph map;
    GraphView* graphView;
    QTableWidget* table;
    QTextEdit* answer;
    QLineEdit* startEdit;
    QLineEdit* finishEdit;

    void buildScreen();
    void reloadTable();
    void redraw();
    void addLine(const QString& text);
    int startNode() const;
    int finishNode() const;
    bool askArc(int& from, int& to, int& weight, bool withWeight);
    bool checkNode(int oneBased, const QString& message);
    QString routeText(const QVector<int>& route) const;
};

#endif
