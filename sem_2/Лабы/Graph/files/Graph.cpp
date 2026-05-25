#include "Graph.h"

#include <queue>

WeightedGraph::WeightedGraph(int size) : nodeCount(size) {
    weights.assign(nodeCount, std::vector<int>(nodeCount, NO_EDGE));
    for (int i = 0; i < nodeCount; ++i) {
        weights[i][i] = 0;
    }
}

bool WeightedGraph::valid(int node) const {
    return node >= 0 && node < nodeCount;
}

void WeightedGraph::appendNode() {
    ++nodeCount;

    for (auto& row : weights) {
        row.push_back(NO_EDGE);
    }

    weights.push_back(std::vector<int>(nodeCount, NO_EDGE));
    weights.back()[nodeCount - 1] = 0;
}

void WeightedGraph::deleteNode(int node) {
    if (!valid(node)) {
        return;
    }

    weights.erase(weights.begin() + node);
    for (auto& row : weights) {
        row.erase(row.begin() + node);
    }
    --nodeCount;
}

void WeightedGraph::setArc(int from, int to, int cost) {
    if (!valid(from) || !valid(to)) {
        return;
    }
    weights[from][to] = from == to ? 0 : cost;
}

void WeightedGraph::clearArc(int from, int to) {
    if (!valid(from) || !valid(to)) {
        return;
    }
    weights[from][to] = from == to ? 0 : NO_EDGE;
}

int WeightedGraph::cost(int from, int to) const {
    if (!valid(from) || !valid(to)) {
        return NO_EDGE;
    }
    return weights[from][to];
}

QVector<int> WeightedGraph::breadthRoute(int start) const {
    if (!valid(start)) {
        return {};
    }

    QVector<int> result;
    QVector<bool> seen(nodeCount, false);
    std::queue<int> line;

    seen[start] = true;
    line.push(start);

    while (!line.empty()) {
        int current = line.front();
        line.pop();
        result.push_back(current);

        for (int next = 0; next < nodeCount; ++next) {
            if (current != next && weights[current][next] != NO_EDGE && !seen[next]) {
                seen[next] = true;
                line.push(next);
            }
        }
    }

    return result;
}

QVector<int> WeightedGraph::depthRoute(int start) const {
    if (!valid(start)) {
        return {};
    }

    QVector<int> result;
    QVector<bool> seen(nodeCount, false);
    QVector<int> stack;
    stack.push_back(start);

    while (!stack.isEmpty()) {
        int current = stack.takeLast();
        if (seen[current]) {
            continue;
        }

        seen[current] = true;
        result.push_back(current);

        for (int next = nodeCount - 1; next >= 0; --next) {
            if (current != next && weights[current][next] != NO_EDGE && !seen[next]) {
                stack.push_back(next);
            }
        }
    }

    return result;
}

QVector<int> WeightedGraph::shortestPath(int start, int finish) const {
    if (!valid(start) || !valid(finish)) {
        return {};
    }

    std::vector<int> distance(nodeCount, NO_EDGE);
    std::vector<int> parent(nodeCount, -1);
    std::vector<bool> done(nodeCount, false);
    distance[start] = 0;

    for (int step = 0; step < nodeCount; ++step) {
        int current = -1;
        for (int node = 0; node < nodeCount; ++node) {
            if (!done[node] && (current == -1 || distance[node] < distance[current])) {
                current = node;
            }
        }

        if (current == -1 || distance[current] == NO_EDGE) {
            break;
        }

        done[current] = true;
        for (int next = 0; next < nodeCount; ++next) {
            if (weights[current][next] == NO_EDGE) {
                continue;
            }

            int candidate = distance[current] + weights[current][next];
            if (candidate < distance[next]) {
                distance[next] = candidate;
                parent[next] = current;
            }
        }
    }

    if (distance[finish] == NO_EDGE) {
        return {};
    }

    QVector<int> path;
    for (int node = finish; node != -1; node = parent[node]) {
        path.push_front(node);
    }
    return path;
}

std::vector<std::vector<int>> WeightedGraph::allDistances() const {
    std::vector<std::vector<int>> distance = weights;
    for (int i = 0; i < nodeCount; ++i) {
        distance[i][i] = 0;
    }

    for (int middle = 0; middle < nodeCount; ++middle) {
        for (int from = 0; from < nodeCount; ++from) {
            for (int to = 0; to < nodeCount; ++to) {
                if (distance[from][middle] == NO_EDGE || distance[middle][to] == NO_EDGE) {
                    continue;
                }

                int candidate = distance[from][middle] + distance[middle][to];
                if (candidate < distance[from][to]) {
                    distance[from][to] = candidate;
                }
            }
        }
    }

    return distance;
}
