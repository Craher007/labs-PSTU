#ifndef GRAPHCANVAS_H
#define GRAPHCANVAS_H

#include <QPointF>
#include <QVector>
#include <QWidget>

#include "branchboundsolver.h"

class QMouseEvent;
class QPainter;

class GraphCanvas : public QWidget {
public:
    enum Mode {
        MoveMode,
        AddEdgeMode,
        RemoveEdgeMode
    };

    explicit GraphCanvas(QWidget* parent = nullptr);

    QVector<QVector<int>> graph() const;
    void setMode(Mode newMode);
    void setEdgeWeight(int value);
    void addNode(const QPointF& position);
    void addNodeAuto();
    void clearGraph();
    void loadDemo();
    void setRoute(const QVector<int>& newRoute);

protected:
    void paintEvent(QPaintEvent*) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent*) override;

private:
    QVector<QPointF> points;
    QVector<QVector<int>> weights;
    QVector<int> route;
    Mode mode = MoveMode;
    int edgeWeight = 1;
    int firstVertex = -1;
    int draggedVertex = -1;
    int pressedVertex = -1;
    int hoverVertex = -1;
    bool movedAfterPress = false;
    QPointF pressPosition;
    QPointF dragShift;
    const int nodeRadius = 24;
    const int minNodeGap = 76;

    int vertexAt(const QPointF& position) const;
    QPointF keepInsideCanvas(const QPointF& point) const;
    QPointF bestFreePosition(const QPointF& preferred) const;
    bool isRouteEdge(int a, int b) const;
    void handleVertexClick(int vertex);
    void addEdge(int a, int b, int value);
    void removeEdge(int a, int b);
    void drawBackground(QPainter& painter);
    void drawEdges(QPainter& painter, bool highlighted);
    void drawNodes(QPainter& painter);
};

#endif
