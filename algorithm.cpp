#include <QDebug>
#include "algorithm.h"
#define DEBUG_MODE true

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


/***************
 * [算法] 进行多边形裁剪（Weiler-Atherton Algorithm）
 * 调用时需保证多边形 A、B 合法
 ***************/
void startClipPolygon(Polygons &polygonsA, Polygons &polygonsB, Polygons &polygonsC) {
    // *** 初始化 ***
    polygonsC.clear();
    if (polygonsA.size() == 0 || polygonsB.size() == 0)
        return;
    if (DEBUG_MODE) {
        qDebug() << "@Debug | ***** WA Begin ***** " << Qt::endl;
    }
    // *** 创建边列表 O(m+n) ***
    vector<Line> lineListA, lineListB;
    // * A 外环入表 - 逆时针 *
    {
        Polygon &polygon = polygonsA[0];
        if (!checkPolygonClockWise(polygon)) for (int i = 0, j = polygon.size() - 1; i < int(polygon.size()); j = i++) lineListA.push_back(Line(polygon[j], polygon[i]));
        else for (int i = polygon.size() - 1, j = 0; i >= 0; j = i--) lineListA.push_back(Line(polygon[j], polygon[i]));
    }
    // * A 内环入表 - 顺时针 *
    for (int k = 1; k < int(polygonsA.size()) && polygonsA[k].size(); k++) {
        Polygon &polygon = polygonsA[k];
        if (checkPolygonClockWise(polygon)) for (int i = 0, j = polygon.size() - 1; i < int(polygon.size()); j = i++) lineListA.push_back(Line(polygon[j], polygon[i]));
        else for (int i = polygon.size() - 1, j = 0; i >= 0; j = i--) lineListA.push_back(Line(polygon[j], polygon[i]));
    }
    // * B 外环入表 - 逆时针 *
    {
        Polygon &polygon = polygonsB[0];
        if (!checkPolygonClockWise(polygon)) for (int i = 0, j = polygon.size() - 1; i < int(polygon.size()); j = i++) lineListB.push_back(Line(polygon[j], polygon[i]));
        else for (int i = polygon.size() - 1, j = 0; i >= 0; j = i--) lineListB.push_back(Line(polygon[j], polygon[i]));
    }
    // * B 内环入表 - 顺时针 *
    for (int k = 1; k < int(polygonsB.size()) && polygonsB[k].size(); k++) {
        Polygon &polygon = polygonsB[k];
        if (checkPolygonClockWise(polygon)) for (int i = 0, j = polygon.size() - 1; i < int(polygon.size()); j = i++) lineListB.push_back(Line(polygon[j], polygon[i]));
        else for (int i = polygon.size() - 1, j = 0; i >= 0; j = i--) lineListB.push_back(Line(polygon[j], polygon[i]));
    }
    if (lineListA.size() == 0 || lineListB.size() == 0)
        return;
    if (DEBUG_MODE) {
        qDebug() << "@Debug | A 边数: " << lineListA.size();
        qDebug() << "@Debug | B 边数: " << lineListB.size();
    }
    // *** 求取边交点 O(mn) ***
    int m = lineListA.size(), n = lineListB.size();
    // TODO 向量叉积

    // *** 创建点列表 O(m+n) ***
    CPoint *cpointListA = nullptr, *cpointListB = nullptr;
    CPoint *cpointListTailA = nullptr, *cpointListTailB = nullptr;
    // * A 遍历边列表 *
    for (Line &line : lineListA) {
        // 插入顶点
        if (cpointListA) { cpointListTailA->next = new CPoint(double(line.begin.x), double(line.begin.y)); cpointListTailA = cpointListTailA->next; }
        else { cpointListA = cpointListTailA = new CPoint(double(line.begin.x), double(line.begin.y)); }
        // 插入交点
        cpointListTailA->next = line.cpointList;
        line.cpointList = nullptr;
        while (cpointListTailA->next) cpointListTailA = cpointListTailA->next;
    }
    cpointListTailA->next = new CPoint(double(lineListA[0].begin.x), double(lineListA[0].begin.y));
    cpointListTailA = cpointListTailA->next;
    // * B 遍历边列表 *
    for (Line &line : lineListB) {
        // 插入顶点
        if (cpointListB) { cpointListTailB->next = new CPoint(double(line.begin.x), double(line.begin.y)); cpointListTailB = cpointListTailB->next; }
        else { cpointListB = cpointListTailB = new CPoint(double(line.begin.x), double(line.begin.y)); }
        // 插入交点
        cpointListTailB->next = line.cpointList;
        line.cpointList = nullptr;
        while (cpointListTailB->next) cpointListTailB = cpointListTailB->next;
    }
    cpointListTailB->next = new CPoint(double(lineListB[0].begin.x), double(lineListB[0].begin.y));
    cpointListTailB = cpointListTailB->next;
    if (!cpointListA || !cpointListB)
        return;
    if (DEBUG_MODE) {
        CPoint *p = cpointListA;
        qDebug() << "@Debug | A 顶点/交点列表: \n (" << p->x << "," << p->y << "," << bool(p->other) << ")";
        while ((p = p->next)) qDebug() << " (" << p->x << "," << p->y << "," << bool(p->other) << ")";
        p = cpointListB;
        qDebug() << "@Debug | B 顶点/交点列表: \n (" << p->x << "," << p->y << "," << bool(p->other) << ")";
        while ((p = p->next)) qDebug() << " (" << p->x << "," << p->y << "," << bool(p->other) << ")";
    }
    // *** 执行裁剪算法 O(m+n) ***
    // TODO

    // *** 返回裁剪结果 O(?) ***


    if (DEBUG_MODE) {
        qDebug() << "@Debug | ***** WA End ***** " << Qt::endl;
    }
    return;
}

