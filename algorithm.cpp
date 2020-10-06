#include <QDebug>
#include <math.h>
#include "algorithm.h"
#define DEBUG_MODE true

using namespace std;



/***************
 * [算法] 判断多边形（无内环）顶点是否顺时针序
 ***************/
bool checkPolygonClockWise(const Polygon &polygon) {
    double S = 0;
    for (int i = 0, j = polygon.size() - 1; i < int(polygon.size()); j = i++)
        S += polygon[j].x * polygon[i].y - polygon[j].y * polygon[i].x;
    return S >= 0;
}


/***************
 * [算法] 判断点是否在多边形（无内环）内部
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
bool checkLineWithLine(const Line &lineA, const Line &lineB) {
    const Point &a = lineA.begin, &b = lineA.end, &c = lineB.begin, &d = lineB.end;
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
 * [算法] 判断多边形（无内环）对于多边形（带内环）是否有效
 * 若多边形 A 与 多边形 B 无交，且 A 所有顶点在 B 外环外，或 A 所有顶点在 B 内环内
 * 则 A、B 显然无交，A 对于 B 无效
 ***************/
bool checkPolygonWithPolygons(const Polygon &polygon, const Polygons &polygons) {
    // 检测单个顶点即可，无需检测所有顶点
    if (polygon.size() == 0) { return false; }
    const Point &point = polygon[0];
    if (!checkPointInPolygon(point, polygons[0])) { return false; }
    for (int i = 1; i < int(polygons.size()); i++) { if (checkPointInPolygon(point, polygons[i])) { return false; } }
    return true;
}


/***************
 * [算法] 判断两条线段是否有交点
 ***************/
bool checkCrossPoint(const Line &lineA, const Line &lineB) {
    if ((lineA.begin.x == lineB.begin.x && lineA.begin.y == lineB.begin.y)
            || (lineA.begin.x == lineB.end.x && lineA.begin.y == lineB.end.y)
            || (lineA.end.x == lineB.begin.x && lineA.end.y == lineB.begin.y)
            || (lineA.end.x == lineB.end.x && lineA.end.y == lineB.end.y))
        return true;
    return checkLineWithLine(lineA, lineB);
}


/***************
 * [算法] 计算两条线段的交点
 ***************/
CPoint* calculateCrossPoint(const Line &lineA, const Line &lineB) {
    const Point &a = lineA.begin, &b = lineA.end, &c = lineB.begin, &d = lineB.end;
    // *** 计算交点 ***
    double D = (b.x - a.x) * (d.y - c.y) - (d.x - c.x) * (b.y - a.y);
    if (D == 0) return nullptr;
    double b1 = (b.y - a.y) * a.x + (a.x - b.x) * a.y;
    double b2 = (d.y - c.y) * c.x + (c.x - d.x) * c.y;
    double x = (b2 * (b.x - a.x) - b1 * (d.x - c.x)) / D, y = (b2 * (b.y - a.y) - b1 * (d.y - c.y)) / D;
    // *** 创建交点元素 ***
    CPoint *first = new CPoint(x, y);
    if (!first) return nullptr;
    CPoint *second = new CPoint(x, y);
    if (!second) return nullptr;
    first->other = second;
    second->other = first;
    // * 计算 t 参数 *
    first->t = (b.x - a.x != 0 ? double(x - a.x) / double (b.x - a.x) : double(y - a.y) / double (b.y - a.y));
    second->t = (d.x - c.x != 0 ? double(x - c.x) / double (d.x - c.x) : double(y - c.y) / double (d.y - c.y));
    // * 判断入点/出点 *
    // 注意：对于外环（逆时针）, 线段向量(x, y), 则(-y, x)恰为其外法向量（界面坐标系下）
    //      对于内环（顺时针）, 线段向量(x, y), 则(-y, x)恰为其内法向量（界面坐标系下）
    // 将主多边形线段与裁剪多边形线段外/内法向量做点积，若结果为负，交点为入点; 若结果为正，交点为出点
    int xF = c.y - d.y, yF = d.x - c.x;
    first->isEntry = second->isEntry = ((b.x - a.x) * xF + (b.y - a.y) * yF < 0);

    qDebug() << "@Debug | " << QString("(%1,%2)(%3,%4)->(%5,%6)(%7,%8)   (%9, %10)(%11,%12)")
                .arg(a.x).arg(a.y).arg(b.x).arg(b.y).arg(c.x).arg(c.y).arg(d.x).arg(d.y).arg(b.x - a.x).arg(b.y - a.y).arg(xF).arg(yF)
             << ((b.x - a.x) * xF + (b.y - a.y) * yF < 0);

    // *** 返回交点元素 ***
    return first;
}


/***************
 * [算法] 进行多边形（带内环）裁剪（Weiler-Atherton Algorithm）
 * 调用时需保证多边形 A、B 合法
 ***************/
int startClipPolygon(Polygons &polygonsA, Polygons &polygonsB, Polygons &polygonsC) {
    // *** 初始化 ***
    polygonsC.clear();
    if (polygonsA.size() == 0 || polygonsB.size() == 0)
        return 1;
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
        if (lineListA.size() > 0) lineListA.back().isEndLine = true;
    }
    // * A 内环入表 - 顺时针 *
    for (int k = 1; k < int(polygonsA.size()) && polygonsA[k].size(); k++) {
        Polygon &polygon = polygonsA[k];
        if (checkPolygonClockWise(polygon)) for (int i = 0, j = polygon.size() - 1; i < int(polygon.size()); j = i++) lineListA.push_back(Line(polygon[j], polygon[i]));
        else for (int i = polygon.size() - 1, j = 0; i >= 0; j = i--) lineListA.push_back(Line(polygon[j], polygon[i]));
        if (lineListA.size() > 0) lineListA.back().isEndLine = true;
    }
    // * B 外环入表 - 逆时针 *
    {
        Polygon &polygon = polygonsB[0];
        if (!checkPolygonClockWise(polygon)) for (int i = 0, j = polygon.size() - 1; i < int(polygon.size()); j = i++) lineListB.push_back(Line(polygon[j], polygon[i]));
        else for (int i = polygon.size() - 1, j = 0; i >= 0; j = i--) lineListB.push_back(Line(polygon[j], polygon[i]));
        if (lineListB.size() > 0) lineListB.back().isEndLine = true;
    }
    // * B 内环入表 - 顺时针 *
    for (int k = 1; k < int(polygonsB.size()) && polygonsB[k].size(); k++) {
        Polygon &polygon = polygonsB[k];
        if (checkPolygonClockWise(polygon)) for (int i = 0, j = polygon.size() - 1; i < int(polygon.size()); j = i++) lineListB.push_back(Line(polygon[j], polygon[i]));
        else for (int i = polygon.size() - 1, j = 0; i >= 0; j = i--) lineListB.push_back(Line(polygon[j], polygon[i]));
        if (lineListB.size() > 0) lineListB.back().isEndLine = true;
    }
    if (lineListA.size() == 0 || lineListB.size() == 0)
        return 2;
    if (DEBUG_MODE) {
        qDebug() << "@Debug | A 边数: " << lineListA.size();
        qDebug() << "@Debug | B 边数: " << lineListB.size();
    }
    // *** 求取边交点 O(mn) ***
    for (Line &lineA : lineListA)
        for (Line &lineB : lineListB)
            if (checkCrossPoint(lineA, lineB)) {
                CPoint *p = calculateCrossPoint(lineA, lineB);
                if (p) { lineA.insertCPoint(p); lineB.insertCPoint(p->other); }
            }
    // *** 无交点环特判 ***
    // 若无交点环顶点完全在另一多边形外环外或内环内，此环可忽略；否则此环需加入结果多边形
    {
        // * A 中无交点环 *
        int m = lineListA.size(), n = lineListB.size();
        int k = 0, i = 0, j = 0;
        while (j < m) {
            while (j < m && !lineListA[j].isEndLine) j++;
            if (j < m) {
                bool noCrossPoint = true;
                for (int x = i; x <= j; x++)
                    if (lineListA[x].cpointList) { noCrossPoint = false; break; }
                if (noCrossPoint && checkPolygonWithPolygons(polygonsA[k], polygonsB))
                    polygonsC.push_back(polygonsA[k]);
            }
            i = j = j + 1;
            k++;
        }
        // * B 中无交点环 *
        k = 0, i = 0, j = 0;
        while (j < n) {
            while (j < n && !lineListB[j].isEndLine) j++;
            if (j < n) {
                bool noCrossPoint = true;
                for (int x = i; x <= j; x++)
                    if (lineListB[x].cpointList) { noCrossPoint = false; break; }
                if (noCrossPoint && checkPolygonWithPolygons(polygonsB[k], polygonsA))
                    polygonsC.push_back(polygonsB[k]);
            }
            i = j = j + 1;
            k++;
        }
    }
    // *** 创建点列表 O(m+n) ***
    CPoint *cpointListA = nullptr, *cpointListB = nullptr;
    CPoint *cpointListHeadA = nullptr, *cpointListHeadB = nullptr;
    CPoint *cpointListTailA = nullptr, *cpointListTailB = nullptr;
    // * A 遍历边列表 *
    for (Line &line : lineListA) {
        // 插入顶点
        if (cpointListA) { cpointListTailA->next = new CPoint(double(line.begin.x), double(line.begin.y)); cpointListTailA = cpointListTailA->next; }
        else { cpointListA = cpointListTailA = new CPoint(double(line.begin.x), double(line.begin.y)); }
        if (!cpointListHeadA) { cpointListHeadA =  cpointListTailA; }
        // 插入交点
        cpointListTailA->next = line.cpointList;
        line.cpointList = nullptr;
        while (cpointListTailA->next) cpointListTailA = cpointListTailA->next;
        // 处理终边
        if (line.isEndLine) {
            cpointListTailA->next = new CPoint(double(line.end.x), double(line.end.y));
            cpointListTailA = cpointListTailA->next;
            cpointListTailA->link = cpointListHeadA;
            cpointListHeadA = nullptr;
        }
    }
    // * B 遍历边列表 *
    for (Line &line : lineListB) {
        // 插入顶点
        if (cpointListB) { cpointListTailB->next = new CPoint(double(line.begin.x), double(line.begin.y)); cpointListTailB = cpointListTailB->next; }
        else { cpointListB = cpointListTailB = new CPoint(double(line.begin.x), double(line.begin.y)); }
        if (!cpointListHeadB) { cpointListHeadB =  cpointListTailB; }
        // 插入交点
        cpointListTailB->next = line.cpointList;
        line.cpointList = nullptr;
        while (cpointListTailB->next) cpointListTailB = cpointListTailB->next;
        // 处理终边
        if (line.isEndLine) {
            cpointListTailB->next = new CPoint(double(line.end.x), double(line.end.y));
            cpointListTailB = cpointListTailB->next;
            cpointListTailB->link = cpointListHeadB;
            cpointListHeadB = nullptr;
        }
    }
    if (!cpointListA || !cpointListB)
        return 3;
    if (DEBUG_MODE) {
        CPoint *p = cpointListA;
        qDebug() << "@Debug | A 顶点/交点列表: \n (" << p->x << "," << p->y << "," << (p->other ? "交点" : "顶点") << (p->isEntry ? "入点" : "出点") << ")";
        while ((p = p->next)) qDebug() << " (" << p->x << "," << p->y << "," << (p->other ? "交点" : "顶点") << (p->isEntry ? "入点" : "出点") << ")";
        p = cpointListB;
        qDebug() << "@Debug | B 顶点/交点列表: \n (" << p->x << "," << p->y << "," << (p->other ? "交点" : "顶点") << (p->isEntry ? "入点" : "出点") << ")";
        while ((p = p->next)) qDebug() << " (" << p->x << "," << p->y << "," << (p->other ? "交点" : "顶点") << (p->isEntry ? "入点" : "出点") << ")";
    }
    // *** 执行裁剪算法 O(m+n) ***
    bool isInA = true;
    CPoint *h = cpointListA, *p = nullptr;
    int currentPolygonIndexC = polygonsC.size();
    polygonsC.push_back(Polygon());
    // * 寻找首个交点 *
    if (DEBUG_MODE) {
        qDebug() << QString("@Debug | Begin ---");
    }
    while (h && !h->other) h = h->next;
    if (h) {
        p = h;
        isInA = true;
        while (p) {
            // * 记录当前顶点/交点 *
            if (DEBUG_MODE) {
                qDebug() << QString("@Debug | Insert: (%1, %2, %3)").arg(p->x).arg(p->y).arg(isInA ? "A" : "B");
            }
            polygonsC[currentPolygonIndexC].push_back(Point(round(p->x), round(p->y)));
            p->isVisited = true;
            // * 搜索下一顶点/交点 *
            if (p->other && (p->isEntry != isInA)) { p = p->other; isInA = !isInA; }
            else if (p->link) { p = p->link; }
            p = p->next;
            // * 判断是否回到起点 *
            if (p == h || p->other == h) {
                // 寻找下一未跟踪交点
                while (h && (h->isVisited || !h->other)) h = h->next;
                if (!h) break;
                p = h;
                isInA = true;
                // 建立新结果顶点表
                currentPolygonIndexC++;
                polygonsC.push_back(Polygon());
                if (DEBUG_MODE) {
                    qDebug() << QString("@Debug | End ---");
                    qDebug() << QString("@Debug | Begin ---");
                }
            }
        }
    }
    if (DEBUG_MODE) {
        qDebug() << QString("@Debug | End ---");
    }
    // *** 返回裁剪结果 O(?) ***
    {
        CPoint *q = nullptr, *p = cpointListA;
        while (p) { q = p; p = p->next; delete q; }
        p = cpointListB;
        while (p) { q = p; p = p->next; delete q; }
    }
    if (DEBUG_MODE) {
        qDebug() << "@Debug | ***** WA End ***** " << Qt::endl;
    }
    return 0;
}
