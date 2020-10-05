#ifndef ALGORITHM_H
#define ALGORITHM_H
#include <vector>

#include "define.h"

// [算法] 判断多边形顶点是否顺时针序
bool checkPolygonClockWise(const Polygon &polygon);

// [算法] 判断点是否在多边形内部
bool checkPointInPolygon(const Point &point, const Polygon &polygon);

// [算法] 判断两条线段是否规范相交
bool checkLineWithLine(const Line &line1, const Line &line2);

// [算法] 判断两条线段是否普通相交
bool checkCrossPoint(const Line &line1, const Line &line2);

// [算法] 计算两条线段的交点
CPoint* calculateCrossPoint(const Line &line1, const Line &line2);

// [算法] 进行多边形裁剪
void startClipPolygon(Polygons &polygonsA, Polygons &polygonsB, Polygons &polygonsC);

#endif // ALGORITHM_H
