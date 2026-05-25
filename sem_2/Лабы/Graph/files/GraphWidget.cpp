#include "GraphWidget.h"

#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QPainter>
#include <QPen>
#include <algorithm>
#include <cmath>

namespace {
const double NODE_RADIUS = 22.0;
}

GraphView::GraphView(QWidget *parent) : QGraphicsView(parent),
    graph(nullptr),
    canvas(new QGraphicsScene(this)) {
    setScene(canvas);
    setRenderHint(QPainter::Antialiasing);
    setBackgroundBrush(QColor(245, 247, 250));
    setFrameShape(QFrame::NoFrame);
}

void GraphView::setGraph(WeightedGraph* source) {
    graph = source;
    rebuild();
}

void GraphView::rebuild() {
    canvas->clear();
    if (!graph) {
        return;
    }

    placeNodes();
    paintGraph();
}

void GraphView::placeNodes() {
    int count = graph->size();
    places.resize(count);
    if (count == 0) {
        return;
    }

    double viewWidth = std::max(600, width());
    double viewHeight = std::max(360, height());
    double radius = std::min(viewWidth, viewHeight) * 0.34;
    QPointF center(viewWidth / 2.0, viewHeight / 2.0);

    for (int i = 0; i < count; ++i) {
        double angle = -M_PI / 2 + i * 2 * M_PI / count;
        places[i] = center + QPointF(std::cos(angle) * radius, std::sin(angle) * radius);
    }
}

void GraphView::paintGraph() {
    int count = graph->size();
    if (count == 0) {
        return;
    }

    QPen edgePen(QColor(74, 101, 130), 2);
    QPen nodePen(QColor(38, 54, 71), 2);
    QBrush nodeBrush(Qt::white);

    for (int from = 0; from < count; ++from) {
        for (int to = 0; to < count; ++to) {
            int value = graph->cost(from, to);
            if (from == to || value == NO_EDGE) {
                continue;
            }

            QPointF first = places[from];
            QPointF second = places[to];
            canvas->addLine(QLineF(first, second), edgePen);

            QPointF middle = (first + second) / 2;
            QGraphicsTextItem* text = canvas->addText(QString::number(value));
            QRectF box = text->boundingRect();
            text->setDefaultTextColor(QColor(38, 54, 71));
            text->setPos(middle.x() - box.width() / 2, middle.y() - box.height() / 2);
        }
    }

    for (int i = 0; i < count; ++i) {
        QPointF point = places[i];
        canvas->addEllipse(point.x() - NODE_RADIUS, point.y() - NODE_RADIUS,
                           NODE_RADIUS * 2, NODE_RADIUS * 2, nodePen, nodeBrush);

        QGraphicsTextItem* number = canvas->addText(QString::number(i + 1));
        QRectF box = number->boundingRect();
        number->setDefaultTextColor(QColor(31, 42, 55));
        number->setPos(point.x() - box.width() / 2, point.y() - box.height() / 2);
    }

    canvas->setSceneRect(0, 0, std::max(600, width()), std::max(360, height()));
}

void GraphView::resizeEvent(QResizeEvent *event) {
    QGraphicsView::resizeEvent(event);
    rebuild();
}
