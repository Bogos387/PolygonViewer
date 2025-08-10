#ifndef EDGE_ANALYZER_H
#define EDGE_ANALYZER_H

#include <QLineF>
#include <QVector>


class EdgeAnalyzer
{
public:
    EdgeAnalyzer();

    /// Возвращает перекрывающиеся отрезки/рёбра
    QVector<QLineF> findOverlappingParts(const QLineF& seg1, const QLineF& seg2);


private:
    /// Находит все точки пересечения/перекрытия между двумя отрезками
    QVector<QPointF> getSegmentIntersections_(const QLineF& seg1, const QLineF& seg2);

    /// Проверяет, лежит ли точка на отрезке
    bool isPointOnSegment_(const QPointF& p, const QLineF& segment);

};

#endif // EDGE_ANALYZER_H
