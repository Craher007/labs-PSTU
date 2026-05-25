#ifndef BRANCHBOUNDSOLVER_H
#define BRANCHBOUNDSOLVER_H

#include <QString>
#include <QVector>

const int INF = 100000000;

struct Solution {
    bool ok = false;
    int cost = INF;
    QVector<int> path;
    QString log;
};

struct BBNode {
    QVector<QVector<int>> matrix;
    QVector<int> path;
    int level = 0;
    int city = 0;
    int bound = 0;
    int realCost = 0;
};

class BranchBoundSolver {
public:
    static Solution solve(const QVector<QVector<int>>& graph);

private:
    static bool contains(const QVector<int>& path, int city);
    static int reduceMatrix(QVector<QVector<int>>& matrix);
    static BBNode createChild(const BBNode& parent, int to, const QVector<QVector<int>>& graph);
    static QString pathToString(const QVector<int>& path);
};

#endif
