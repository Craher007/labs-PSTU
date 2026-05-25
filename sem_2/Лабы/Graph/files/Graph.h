#ifndef GRAPH_H
#define GRAPH_H

#include <QVector>
#include <limits>
#include <vector>

const int NO_EDGE = std::numeric_limits<int>::max() / 4;

class WeightedGraph {
public:
    explicit WeightedGraph(int size = 0);

    int size() const { return nodeCount; }
    bool valid(int node) const;

    void appendNode();
    void deleteNode(int node);
    void setArc(int from, int to, int cost);
    void clearArc(int from, int to);
    int cost(int from, int to) const;

    QVector<int> breadthRoute(int start) const;
    QVector<int> depthRoute(int start) const;
    QVector<int> shortestPath(int start, int finish) const;
    std::vector<std::vector<int>> allDistances() const;

private:
    int nodeCount;
    std::vector<std::vector<int>> weights;
};

#endif
