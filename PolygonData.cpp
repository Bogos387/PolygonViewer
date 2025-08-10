#include "PolygonData.h"

#include <QFile>
#include <QDomDocument>
#include <QDebug>



//--------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------
PolygonData::PolygonData()
{

}


//--------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------
void PolygonData::loadFromFile(const QString &filename)
{
    polygons_.clear();

    loadFromXml_(filename);
}


//--------------------------------------------------------------------------------
//  Возвращает все рёбера всех полигонов
//--------------------------------------------------------------------------------
QVector<QLineF> PolygonData::getAllEdges() const
{
    QVector<QLineF> edges;
    for (const QPolygonF &polygon : polygons_)
    {
        for (int i = 1; i < polygon.size(); ++i)
        {
            edges.append(QLineF(polygon[i-1], polygon[i]));
        }
    }
    return edges;
}


//--------------------------------------------------------------------------------
//  Возвращает все полигоны
//--------------------------------------------------------------------------------
const QMap<QString, QPolygonF> &PolygonData::getPolygons() const
{
    return polygons_;
}


//--------------------------------------------------------------------------------
//  Загрузка данных из XML
//--------------------------------------------------------------------------------
void PolygonData::loadFromXml_(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "Failed to open file:" << filename;
        return;
    }

    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        qWarning() << "Failed to parse XML";
        return;
    }

    QDomElement geometryElement = doc.documentElement();
    parseXml_(geometryElement);
}


//--------------------------------------------------------------------------------
//  Заполнение загруженных данных
//--------------------------------------------------------------------------------
void PolygonData::parseXml_(const QDomElement &geometryElement)
{
    QDomNodeList figureNodes = geometryElement.elementsByTagName("figure");
    for (int i = 0; i < figureNodes.count(); ++i)
    {
        QDomElement figure = figureNodes.at(i).toElement();
        QString name = figure.attribute("name");

        QPolygonF polygon;
        QDomNodeList ptNodes = figure.elementsByTagName("pt");
        for (int j = 0; j < ptNodes.count(); ++j)
        {
            QDomElement pt = ptNodes.at(j).toElement();
            double x = pt.attribute("x").toDouble();
            double y = pt.attribute("y").toDouble();
            polygon << QPointF(x, y);
        }
        polygons_[name].append(polygon);
    }
}

