#include <QApplication>

#include "PolygonViewer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    PolygonViewer viewer;
    viewer.resize(800, 600);
    viewer.loadFromFile("../geometry.xml");
    viewer.viewPolygons();
    viewer.highlightEdgeOverlaps();
    viewer.show();


    return a.exec();
}
