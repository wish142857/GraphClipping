#include "algorithm.h"

using namespace std;


/***************
 * [算法] 判断点是否在多边形内部
 ***************/
bool checkPointInPolygon(const Point &point, const Polygon &polygon) {
    int i = 0, j = polygon.size() - 1;
    bool oddNodes = false;

    for (i = 0 ; i < int(polygon.size()); i++) {
        if ((polygon[i].y < point.y && polygon[j].y >= point.y) || (polygon[i].y >= point.y && polygon[j].y < point.y))
            oddNodes ^= (double(polygon[i].x + (point.y - polygon[i].y)) / double((polygon[j].y - polygon[i].y)) * double((polygon[j].x - polygon[i].x)) < double(point.x));
        j = i;
    }
    return oddNodes;
}


/***************
 * [算法] 判断两条线段是否规范相交
 ***************/
bool checkLineWithLine(const Line &line1, const Line &line2) {
    int a = (line1.end.x - line1.begin.x) * (line2.begin.y - line1.begin.y) - (line1.end.y - line1.begin.y) * (line2.begin.x - line1.begin.x);
    int b = (line1.end.x - line1.begin.x) * (line2.end.y - line1.begin.y) -  (line1.end.y - line1.begin.y) * (line2.end.x - line1.begin.x);
    if (a * b >= 0)
        return false;
    a = (line2.end.x - line2.begin.x) * (line1.begin.y - line2.begin.y) - (line2.end.y - line2.begin.y) * (line1.begin.x - line2.begin.x);
    b = (line2.end.x - line2.begin.x) * (line1.end.y - line2.begin.y) - (line2.end.y - line2.begin.y) * (line1.end.x - line2.begin.x);
    if (a * b >= 0)
        return false;
    return true;
}
