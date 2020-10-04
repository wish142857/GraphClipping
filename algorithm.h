#ifndef ALGORITHM_H
#define ALGORITHM_H
#include <vector>

#include "define.h"


bool checkPolygonClockWise(const Polygon &polygon);                     // [算法] 判断多边形顶点是否顺时针序

bool checkPointInPolygon(const Point &point, const Polygon &polygon);   // [算法] 判断点是否在多边形内部

bool checkLineWithLine(const Line &line1, const Line &line2);           // [算法] 判断两条线段是否规范相交


#endif // ALGORITHM_H
