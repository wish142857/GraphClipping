#ifndef ALGORITHM_H
#define ALGORITHM_H
#include <vector>

#include "define.h"


// [算法] 判断多边形（无内环）顶点是否顺时针序
bool checkPolygonClockWise(const Polygon &polygon);

// [算法] 判断点是否在多边形（无内环）内部
bool checkPointInPolygon(const Point &point, const Polygon &polygon);

// [算法] 判断两条线段是否规范相交
bool checkLineWithLine(const Line &lineA, const Line &lineB);

// [算法] 判断多边形（无内环）对于多边形（带内环）是否有效
bool checkPolygonWithPolygons(const Polygon &polygon, const Polygons &polygons);

// [算法] 判断两条线段是否有交点
bool checkCrossPoint(const Line &lineA, const Line &lineB);

// [算法] 计算两条线段的交点
CPoint* calculateCrossPoint(const Line &lineA, const Line &lineB);

// [算法] 进行多边形（带内环）裁剪（Weiler-Atherton Algorithm）
int startClipPolygon(Polygons &polygonsA, Polygons &polygonsB, Polygons &polygonsC);

#endif // ALGORITHM_H
