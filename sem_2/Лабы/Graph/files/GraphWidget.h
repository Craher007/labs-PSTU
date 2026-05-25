#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include "Graph.h"

#include <QGraphicsView>

class GraphView : public QGraphicsView {
    Q_OBJECT
public:
    explicit GraphView(QWidget *parent = nullptr);

    void setGraph(WeightedGraph* source);
    void rebuild();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    WeightedGraph* graph;
    QGraphicsScene* canvas;
    QVector<QPointF> places;

    void placeNodes();
    void paintGraph();
};

#endif
