#include <QDebug>
#include "algorithm.h"

using namespace std;


/***************
 * [算法] 判断多边形顶点是否顺时针序
 ***************/
bool checkPolygonClockWise(const Polygon &polygon) {
    double S = 0;
    for (int i = 0, j = polygon.size() - 1; i < int(polygon.size()); j = i++)
        S += polygon[j].x * polygon[i].y - polygon[j].y * polygon[i].x;
    return S >= 0;
}


/***************
 * [算法] 判断点是否在多边形内部
 ***************/
bool checkPointInPolygon(const Point &point, const Polygon &polygon) {
    bool oddNodes = false;
    for (int i = 0, j = polygon.size() - 1; i < int(polygon.size()); j = i++)
        if ((polygon[i].y < point.y && polygon[j].y >= point.y) || (polygon[i].y >= point.y && polygon[j].y < point.y))
            oddNodes ^= (double(point.y - polygon[i].y) / double(polygon[j].y - polygon[i].y) * double(polygon[j].x - polygon[i].x) + double(polygon[i].x) < double(point.x));
    return oddNodes;
}


/***************
 * [算法] 判断两条线段是否规范相交
 ***************/
bool checkLineWithLine(const Line &line1, const Line &line2) {
    const Point &a = line1.begin, &b = line1.end, &c = line2.begin, &d = line2.end;
    int x = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    int y = (b.x - a.x) * (d.y - a.y) - (b.y - a.y) * (d.x - a.x);
    if (x == 0 || y == 0 || (x > 0 && y > 0) || (x < 0 && y < 0))
        return false;
    x = (d.x - c.x) * (a.y - c.y) - (d.y - c.y) * (a.x - c.x);
    y = (d.x - c.x) * (b.y - c.y) - (d.y - c.y) * (b.x - c.x);
    if (x == 0 || y == 0 || (x > 0 && y > 0) || (x < 0 && y < 0))
        return false;
    return true;
}

