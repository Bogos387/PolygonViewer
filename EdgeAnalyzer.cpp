#include "EdgeAnalyzer.h"



//--------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------
EdgeAnalyzer::EdgeAnalyzer()
{

}


//--------------------------------------------------------------------------------
//  Возвращает перекрывающиеся отрезки/рёбра
//--------------------------------------------------------------------------------
QVector<QLineF> EdgeAnalyzer::findOverlappingParts(const QLineF &seg1, const QLineF &seg2)
{
    QVector<QPointF> intersections = getSegmentIntersections_(seg1, seg2);
    QVector<QLineF> overlaps;

    if (intersections.size() == 2) {
        // Полное или частичное перекрытие
        overlaps << QLineF(intersections[0], intersections[1]);
    }
    else if (intersections.size() == 1) {
        // Одна точка пересечения (касание)
        overlaps << QLineF(intersections[0], intersections[0]);
    }

    return overlaps;
}


//--------------------------------------------------------------------------------
//  Находит все точки пересечения/перекрытия между двумя отрезками
//--------------------------------------------------------------------------------
QVector<QPointF> EdgeAnalyzer::getSegmentIntersections_(const QLineF &seg1, const QLineF &seg2)
{
    QVector<QPointF> points;

    // Проверяем все 4 возможные комбинации концов отрезков
    if (isPointOnSegment_(seg1.p1(), seg2)) points << seg1.p1();
    if (isPointOnSegment_(seg1.p2(), seg2)) points << seg1.p2();
    if (isPointOnSegment_(seg2.p1(), seg1)) points << seg2.p1();
    if (isPointOnSegment_(seg2.p2(), seg1)) points << seg2.p2();

    // Удаляем дубликаты
    std::sort(points.begin(), points.end(), [](const QPointF& a, const QPointF& b) {
        return a.x() < b.x() || (qFuzzyIsNull(a.x() - b.x()) && a.y() < b.y());
    });
    points.erase(std::unique(points.begin(), points.end()), points.end());

    return points;
}


//--------------------------------------------------------------------------------
//  Проверяет, лежит ли точка на отрезке
//--------------------------------------------------------------------------------
bool EdgeAnalyzer::isPointOnSegment_(const QPointF &p, const QLineF &segment)
{
    const QPointF& p1 = segment.p1();
    const QPointF& p2 = segment.p2();

    // Проверка коллинеарности (cross - пересечение)
    double cross = (p2.x() - p1.x()) * (p.y() - p1.y()) -
                   (p2.y() - p1.y()) * (p.x() - p1.x());
    if (!qFuzzyIsNull(cross)) return false;

    // Проверка что точка между концами отрезка (не просто коллинеарна, а принадлежит отрезку)
    if (p.x() < qMin(p1.x(), p2.x()) || p.x() > qMax(p1.x(), p2.x())) return false;
    if (p.y() < qMin(p1.y(), p2.y()) || p.y() > qMax(p1.y(), p2.y())) return false;

    return true;
}
