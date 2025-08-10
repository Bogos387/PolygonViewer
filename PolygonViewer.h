#ifndef POLYGON_VIEWER_H
#define POLYGON_VIEWER_H

#include <QGraphicsView>
#include <QMap>

class QGraphicsScene;

#include "PolygonData.h"


class PolygonViewer : public QGraphicsView
{
    Q_OBJECT

public:
    PolygonViewer(QWidget *parent = Q_NULLPTR);

    /// Загрузка полигонов из файла
    void loadFromFile(const QString &filename);

    /// Отобразить полигоны на сцену
    void viewPolygons();

    /// Выделить перекрытия рёбер
    void highlightEdgeOverlaps();


private:
    QGraphicsScene* scene_;
    QGraphicsItemGroup* overlapLayer_;

    PolygonData polygonData_;

    void drawOverlapFon_();

};
#endif // POLYGON_VIEWER_H
