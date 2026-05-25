#include "graphcanvas.h"

#include <QMouseEvent>
#include <QPainter>
#include <algorithm>
#include <cmath>

static const double PI = 3.14159265358979323846;

GraphCanvas::GraphCanvas(QWidget* parent) : QWidget(parent) {
    setMinimumSize(640, 520);
    setMouseTracking(true);
    setCursor(Qt::ArrowCursor);
    loadDemo();
}

QVector<QVector<int>> GraphCanvas::graph() const {
    return weights;
}

void GraphCanvas::setMode(Mode newMode) {
    mode = newMode;
    firstVertex = -1;
    pressedVertex = -1;
    draggedVertex = -1;
    update();
}

void GraphCanvas::setEdgeWeight(int value) {
    edgeWeight = value;
}

void GraphCanvas::addNode(const QPointF& position) {
    points.push_back(bestFreePosition(position));
    int n = points.size();
    for (int i = 0; i < weights.size(); ++i) {
        weights[i].push_back(INF);
    }
    weights.push_back(QVector<int>(n, INF));
    route.clear();
    update();
}

void GraphCanvas::addNodeAuto() {
    QPointF center(width() * 0.5, height() * 0.5);
    QPointF best = center;
    double bestDistance = -1.0;
    int maxRadius = std::max(80, std::min(width(), height()) / 2 - nodeRadius - 20);

    for (int radius = 80; radius <= maxRadius; radius += minNodeGap) {
        int count = std::max(8, int(2 * PI * radius / minNodeGap));
        for (int i = 0; i < count; ++i) {
            double angle = (2 * PI * i / count) + points.size() * 0.37;
            QPointF candidate = keepInsideCanvas(center + QPointF(std::cos(angle) * radius, std::sin(angle) * radius));
            double nearest = 1000000.0;
            for (const QPointF& point : points) {
                nearest = std::min(nearest, QLineF(candidate, point).length());
            }
            if (points.isEmpty() || nearest > bestDistance) {
                bestDistance = nearest;
                best = candidate;
            }
        }
    }

    addNode(best);
}

void GraphCanvas::clearGraph() {
    points.clear();
    weights.clear();
    route.clear();
    firstVertex = -1;
    pressedVertex = -1;
    draggedVertex = -1;
    hoverVertex = -1;
    update();
}

void GraphCanvas::loadDemo() {
    clearGraph();
    QVector<QPointF> demo = {
        {330, 70}, {530, 170}, {500, 390},
        {310, 470}, {110, 350}, {130, 140}
    };
    for (const QPointF& point : demo) {
        addNode(point);
    }

    addEdge(0, 1, 7);
    addEdge(0, 2, 9);
    addEdge(0, 3, 8);
    addEdge(0, 4, 20);
    addEdge(0, 5, 12);
    addEdge(1, 2, 10);
    addEdge(1, 3, 4);
    addEdge(1, 4, 11);
    addEdge(1, 5, 6);
    addEdge(2, 3, 15);
    addEdge(2, 4, 5);
    addEdge(2, 5, 9);
    addEdge(3, 4, 14);
    addEdge(3, 5, 7);
    addEdge(4, 5, 10);
    route.clear();
    update();
}

void GraphCanvas::setRoute(const QVector<int>& newRoute) {
    route = newRoute;
    update();
}

void GraphCanvas::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    drawBackground(painter);

    if (points.isEmpty()) {
        painter.setPen(QColor(80, 86, 98));
        painter.setFont(QFont("Arial", 13));
        painter.drawText(rect(), Qt::AlignCenter, QString::fromUtf8("Граф пуст"));
        return;
    }

    drawEdges(painter, false);
    drawEdges(painter, true);
    drawNodes(painter);
}

void GraphCanvas::mouseDoubleClickEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        addNode(event->pos());
        firstVertex = -1;
    }
}

void GraphCanvas::mousePressEvent(QMouseEvent* event) {
    if (event->button() != Qt::LeftButton) {
        return;
    }

    int vertex = vertexAt(event->pos());
    pressedVertex = vertex;
    pressPosition = event->pos();
    movedAfterPress = false;

    if (vertex < 0) {
        firstVertex = -1;
        update();
        return;
    }

    if (mode == MoveMode) {
        draggedVertex = vertex;
        dragShift = points[vertex] - event->pos();
    }
    update();
}

void GraphCanvas::mouseMoveEvent(QMouseEvent* event) {
    hoverVertex = vertexAt(event->pos());
    setCursor(hoverVertex >= 0 ? Qt::OpenHandCursor : Qt::ArrowCursor);

    if (pressedVertex >= 0 && (event->buttons() & Qt::LeftButton)) {
        if (draggedVertex < 0 && QLineF(pressPosition, event->pos()).length() > 5) {
            draggedVertex = pressedVertex;
            dragShift = points[draggedVertex] - pressPosition;
            movedAfterPress = true;
            firstVertex = -1;
        }
    }

    if (draggedVertex >= 0) {
        setCursor(Qt::ClosedHandCursor);
        points[draggedVertex] = keepInsideCanvas(event->pos() + dragShift);
        route.clear();
        update();
        return;
    }

    update();
}

void GraphCanvas::mouseReleaseEvent(QMouseEvent*) {
    if (pressedVertex >= 0 && !movedAfterPress && draggedVertex < 0) {
        handleVertexClick(pressedVertex);
    }
    pressedVertex = -1;
    draggedVertex = -1;
    movedAfterPress = false;
    setCursor(hoverVertex >= 0 ? Qt::OpenHandCursor : Qt::ArrowCursor);
    update();
}

int GraphCanvas::vertexAt(const QPointF& position) const {
    for (int i = points.size() - 1; i >= 0; --i) {
        if (QLineF(position, points[i]).length() <= nodeRadius) {
            return i;
        }
    }
    return -1;
}

QPointF GraphCanvas::keepInsideCanvas(const QPointF& point) const {
    double margin = nodeRadius + 12;
    double x = std::max(margin, std::min(point.x(), width() - margin));
    double y = std::max(margin, std::min(point.y(), height() - margin));
    return QPointF(x, y);
}

QPointF GraphCanvas::bestFreePosition(const QPointF& preferred) const {
    QPointF base = keepInsideCanvas(preferred);
    bool free = true;
    for (const QPointF& point : points) {
        if (QLineF(base, point).length() < minNodeGap) {
            free = false;
            break;
        }
    }
    if (free) {
        return base;
    }

    QPointF best = base;
    double bestDistance = -1.0;
    for (int radius = minNodeGap; radius <= 260; radius += 18) {
        int count = std::max(12, int(2 * PI * radius / 30));
        for (int i = 0; i < count; ++i) {
            double angle = 2 * PI * i / count;
            QPointF candidate = keepInsideCanvas(base + QPointF(std::cos(angle) * radius, std::sin(angle) * radius));
            double nearest = 1000000.0;
            for (const QPointF& point : points) {
                nearest = std::min(nearest, QLineF(candidate, point).length());
            }
            if (nearest > bestDistance) {
                bestDistance = nearest;
                best = candidate;
            }
            if (nearest >= minNodeGap) {
                return candidate;
            }
        }
    }

    return best;
}

bool GraphCanvas::isRouteEdge(int a, int b) const {
    for (int i = 0; i + 1 < route.size(); ++i) {
        int x = route[i];
        int y = route[i + 1];
        if ((x == a && y == b) || (x == b && y == a)) {
            return true;
        }
    }
    return false;
}

void GraphCanvas::handleVertexClick(int vertex) {
    if (mode == MoveMode) {
        return;
    }

    if (firstVertex < 0) {
        firstVertex = vertex;
    } else if (firstVertex == vertex) {
        firstVertex = -1;
    } else {
        if (mode == AddEdgeMode) {
            addEdge(firstVertex, vertex, edgeWeight);
        } else {
            removeEdge(firstVertex, vertex);
        }
        route.clear();
        firstVertex = -1;
    }
}

void GraphCanvas::addEdge(int a, int b, int value) {
    if (a == b) {
        return;
    }
    weights[a][b] = value;
    weights[b][a] = value;
}

void GraphCanvas::removeEdge(int a, int b) {
    weights[a][b] = INF;
    weights[b][a] = INF;
}

void GraphCanvas::drawBackground(QPainter& painter) {
    painter.fillRect(rect(), QColor(245, 248, 252));
    painter.setPen(QPen(QColor(225, 231, 240), 1));
    for (int x = 0; x < width(); x += 32) {
        painter.drawLine(x, 0, x, height());
    }
    for (int y = 0; y < height(); y += 32) {
        painter.drawLine(0, y, width(), y);
    }

    QRectF area = rect().adjusted(10, 10, -10, -10);
    painter.setPen(QPen(QColor(207, 216, 230), 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(area, 10, 10);
}

void GraphCanvas::drawEdges(QPainter& painter, bool highlighted) {
    for (int i = 0; i < points.size(); ++i) {
        for (int j = i + 1; j < points.size(); ++j) {
            if (weights[i][j] >= INF || isRouteEdge(i, j) != highlighted) {
                continue;
            }

            QColor color = highlighted ? QColor(226, 84, 53) : QColor(132, 145, 166);
            painter.setPen(QPen(color, highlighted ? 5 : 2, Qt::SolidLine, Qt::RoundCap));
            painter.drawLine(points[i], points[j]);

            QPointF middle = (points[i] + points[j]) / 2.0;
            QLineF line(points[i], points[j]);
            QPointF normal(-line.dy(), line.dx());
            double length = std::max(1.0, std::sqrt(normal.x() * normal.x() + normal.y() * normal.y()));
            middle += normal / length * 12.0;
            QRectF labelRect(middle.x() - 18, middle.y() - 13, 36, 26);
            painter.setPen(Qt::NoPen);
            painter.setBrush(highlighted ? QColor(255, 235, 228) : QColor(255, 255, 255));
            painter.drawRoundedRect(labelRect, 6, 6);
            painter.setPen(highlighted ? QColor(165, 46, 29) : QColor(70, 78, 92));
            painter.setFont(QFont("Arial", 10, QFont::Bold));
            painter.drawText(labelRect, Qt::AlignCenter, QString::number(weights[i][j]));
        }
    }
}

void GraphCanvas::drawNodes(QPainter& painter) {
    for (int i = 0; i < points.size(); ++i) {
        QRectF circle(points[i].x() - nodeRadius, points[i].y() - nodeRadius, nodeRadius * 2, nodeRadius * 2);
        QColor fill = i == 0 ? QColor(76, 175, 124) : QColor(57, 105, 198);
        if (i == firstVertex) {
            fill = QColor(245, 168, 63);
        }
        if (i == hoverVertex || i == draggedVertex) {
            painter.setPen(QPen(QColor(95, 121, 180, 90), 8));
            painter.setBrush(Qt::NoBrush);
            painter.drawEllipse(circle.adjusted(-5, -5, 5, 5));
        }

        painter.setPen(QPen(i == firstVertex ? QColor(126, 80, 20) : QColor(255, 255, 255), 3));
        painter.setBrush(fill);
        painter.drawEllipse(circle);
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 12, QFont::Bold));
        painter.drawText(circle, Qt::AlignCenter, QString::number(i + 1));
    }
}
