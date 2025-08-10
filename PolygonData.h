#ifndef POLYGON_DATA_H
#define POLYGON_DATA_H

#include <QPolygonF>
#include <QLineF>
#include <QMap>

class QDomElement;


class PolygonData
{
public:
    PolygonData();

    void loadFromFile(const QString &filename);

    QVector<QLineF> getAllEdges() const;

    const QMap<QString, QPolygonF>& getPolygons() const;


private:
    QMap<QString, QPolygonF> polygons_;

    void loadFromXml_(const QString &filename);
    void parseXml_(const QDomElement &geometryElement);

};

#endif // POLYGON_DATA_H
