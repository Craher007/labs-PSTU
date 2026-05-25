#include "branchboundsolver.h"

#include <QStringList>
#include <algorithm>
#include <queue>

Solution BranchBoundSolver::solve(const QVector<QVector<int>>& graph) {
    Solution result;
    const int n = graph.size();
    if (n < 3) {
        result.log = QString::fromUtf8("Нужно минимум 3 вершины.");
        return result;
    }

    QVector<QVector<int>> matrix = graph;
    for (int i = 0; i < n; ++i) {
        matrix[i][i] = INF;
    }

    BBNode root;
    root.matrix = matrix;
    root.path.push_back(0);
    root.level = 1;
    root.city = 0;
    root.bound = reduceMatrix(root.matrix);

    auto cmp = [](const BBNode& a, const BBNode& b) {
        return a.bound > b.bound;
    };
    std::priority_queue<BBNode, std::vector<BBNode>, decltype(cmp)> queue(cmp);
    queue.push(root);

    result.log += QString::fromUtf8("Старт из вершины 1. Начальная нижняя граница: %1\n").arg(root.bound);

    while (!queue.empty()) {
        BBNode current = queue.top();
        queue.pop();

        if (current.bound >= result.cost) {
            continue;
        }

        if (current.level == n) {
            int last = current.city;
            if (graph[last][0] >= INF) {
                continue;
            }

            int total = current.realCost + graph[last][0];
            if (total < result.cost) {
                result.ok = true;
                result.cost = total;
                result.path = current.path;
                result.path.push_back(0);
                result.log += QString::fromUtf8("Найден лучший цикл: %1, стоимость %2\n")
                    .arg(pathToString(result.path)).arg(result.cost);
            }
            continue;
        }

        for (int next = 0; next < n; ++next) {
            if (contains(current.path, next) || current.matrix[current.city][next] >= INF) {
                continue;
            }

            BBNode child = createChild(current, next, graph);
            if (child.bound < result.cost) {
                queue.push(child);
                result.log += QString::fromUtf8("Добавлен путь %1, граница %2\n")
                    .arg(pathToString(child.path)).arg(child.bound);
            }
        }
    }

    if (!result.ok) {
        result.log += QString::fromUtf8("Гамильтонов цикл не найден. Добавьте недостающие связи.");
    }
    return result;
}

bool BranchBoundSolver::contains(const QVector<int>& path, int city) {
    return std::find(path.begin(), path.end(), city) != path.end();
}

int BranchBoundSolver::reduceMatrix(QVector<QVector<int>>& matrix) {
    int n = matrix.size();
    int reduction = 0;

    for (int i = 0; i < n; ++i) {
        int rowMin = INF;
        for (int j = 0; j < n; ++j) {
            rowMin = std::min(rowMin, matrix[i][j]);
        }
        if (rowMin > 0 && rowMin < INF) {
            reduction += rowMin;
            for (int j = 0; j < n; ++j) {
                if (matrix[i][j] < INF) {
                    matrix[i][j] -= rowMin;
                }
            }
        }
    }

    for (int j = 0; j < n; ++j) {
        int colMin = INF;
        for (int i = 0; i < n; ++i) {
            colMin = std::min(colMin, matrix[i][j]);
        }
        if (colMin > 0 && colMin < INF) {
            reduction += colMin;
            for (int i = 0; i < n; ++i) {
                if (matrix[i][j] < INF) {
                    matrix[i][j] -= colMin;
                }
            }
        }
    }

    return reduction;
}

BBNode BranchBoundSolver::createChild(const BBNode& parent, int to, const QVector<QVector<int>>& graph) {
    BBNode child;
    int from = parent.city;
    int n = graph.size();

    child.matrix = parent.matrix;
    child.path = parent.path;
    child.path.push_back(to);
    child.level = parent.level + 1;
    child.city = to;
    child.realCost = parent.realCost + graph[from][to];

    for (int i = 0; i < n; ++i) {
        child.matrix[from][i] = INF;
        child.matrix[i][to] = INF;
    }
    child.matrix[to][0] = INF;

    child.bound = parent.bound + parent.matrix[from][to] + reduceMatrix(child.matrix);
    return child;
}

QString BranchBoundSolver::pathToString(const QVector<int>& path) {
    QStringList list;
    for (int city : path) {
        list << QString::number(city + 1);
    }
    return list.join(QString::fromUtf8(" -> "));
}
