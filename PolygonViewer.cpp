#include "PolygonViewer.h"

#include <QGraphicsScene>
#include <QGraphicsPolygonItem>
#include <QDebug>

#include "EdgeAnalyzer.h"


//--------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------
PolygonViewer::PolygonViewer(QWidget *parent)
    : QGraphicsView(parent)
    , scene_(new QGraphicsScene(this))
{
    setScene(scene_);
}


//--------------------------------------------------------------------------------
//  Загрузка полигонов из файла
//--------------------------------------------------------------------------------
void PolygonViewer::loadFromFile(const QString &filename)
{
    scene_->clear();

    polygonData_.loadFromFile(filename);
}


//--------------------------------------------------------------------------------
//  Отобразить полигоны на сцену
//--------------------------------------------------------------------------------
void PolygonViewer::viewPolygons()
{
    auto& polygons = polygonData_.getPolygons();
    for (const QString& name : polygons.keys())
    {
        const QPolygonF &polygon = polygons.value(name);
        // Полигон
        QGraphicsPolygonItem *item = scene_->addPolygon(polygon);
        item->setBrush(QColor(100, 100, 255, 100));
        item->setPen(QPen(Qt::black, 1));
        item->setToolTip(name);

        // Текст полигона
        QGraphicsSimpleTextItem* text = scene_->addSimpleText(name);
        QRectF polyRect = polygon.boundingRect();
        text->setPos(polyRect.center() - QPointF(text->boundingRect().width()/2.0,
                                                 text->boundingRect().height()/2.0));
        text->setBrush(QColor(0, 200, 0));
        text->setFont(QFont("Arial", 12));
        text->setParentItem(item);
    }
}


//--------------------------------------------------------------------------------
//  Выделите перекрывающиеся края
//--------------------------------------------------------------------------------
void PolygonViewer::highlightEdgeOverlaps()
{
    overlapLayer_ = scene_->createItemGroup({});
    overlapLayer_->setZValue(2);
    //drawOverlapFon_();

    EdgeAnalyzer edgeAnalyzer;
    // вектор всех отрезков полигонов
    QVector<QLineF> edges = polygonData_.getAllEdges();

    // Сверяем каждый отрезок с каждым
    for (int i = 0; i < edges.size(); ++i)
    {
        for (int j = i + 1; j < edges.size(); ++j)
        {
            QVector<QLineF> overlaps = edgeAnalyzer.findOverlappingParts(edges[i], edges[j]);

            for (const QLineF& overlap : overlaps)
            {
                if (overlap.p1() == overlap.p2())
                {
                    // Точка пересечения
                    scene_->addEllipse(QRectF(overlap.x1() - 2, overlap.y1() - 2, 4, 4),
                                       QPen(Qt::black), QBrush(Qt::black));
                }
                else
                {
                    // Линия перекрытия
                    QGraphicsItem* item = new QGraphicsLineItem(overlap);
                    if (!item) continue;
                    dynamic_cast<QGraphicsLineItem*>(item)->setPen(QPen(Qt::red, 3, Qt::SolidLine));
                    overlapLayer_->addToGroup(item);
                }
            }
        }
    }
    qDebug() << "Количество пересечений:"
             << overlapLayer_->childItems().count();
}


//--------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------
void PolygonViewer::drawOverlapFon_()
{
    QGraphicsRectItem* overlapBack = scene_->addRect(scene_->sceneRect(),
                                                     QPen(Qt::NoPen),
                                                     QBrush(QColor(255, 200, 200, 100)));
    overlapBack->setZValue(1);
}

