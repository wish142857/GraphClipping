#ifndef DEFINE_H
#define DEFINE_H
#include <vector>
#include <string>
#include <QString>

/***************
 * [常量]
 ***************/
const std::string WINDOW_TITLE = "『 iClip 』 -  Ver 1.0"; // 窗口标题
const int WINDOW_WIDTH = 1200;              // 窗口宽度
const int WINDOW_HEIGHT = 900;              // 窗口高度
const int DRAWING_AREA_SIZE = 900;          // 绘图区域边长
const int DRAWING_AREA_X_OFFSET = -300;     // 绘图区域横轴偏移值
const int DRAWING_AREA_Y_OFFSET = 0;        // 绘图区域纵轴偏移值
const int DEFAULT_PEN_WIDTH = 3;            // 默认画笔宽度
const int INFO_MAX_LINE_NUMBER = 100;       // 信息最大行数
const QString SUCCESS_COLOR = "#228B22";    // 成功信息颜色
const QString ERROR_COLOR = "#FF0000";      // 失败信息颜色
const QString WARNING_COLOR = "#FFD700";    // 警告信息颜色

const QString START_BUTTON_TEXT_1 = "开始绘图";
const QString START_BUTTON_TEXT_2 = "正在绘图";
const QString POS_LABEL_TEXT = "当前坐标: (%1, %2)";

const QString INFO_INSERT_POINT_SUCCEED = "[%1] 插入顶点成功: (%2, %3)";
const QString INFO_INSERT_POINT_FAIL_1 = "[%1] 插入顶点失败: 重复顶点 (%2, %3)";
const QString INFO_INSERT_POINT_FAIL_2 = "[%1] 插入顶点失败: 将造成交叉线段\n[(%2,%3)(%4,%5)]×[(%6,%7)(%8,%9)]";
const QString INFO_INSERT_POINT_FAIL_3 = "[%1] 插入顶点失败: 内环顶点需在基本多边形内部 (%2, %3)";
const QString INFO_INSERT_POINT_FAIL_4 = "[%1] 插入顶点失败: 内环顶点不应在其他内环内部 (%2, %3)";

const QString INFO_CANCEL_SUCCEED = "[%1] 撤销顶点成功: (%2, %3)";
const QString INFO_CANCEL_FAIL = "[%1] 撤销顶点失败: 已无剩余顶点";

const QString INFO_CLEAR_SUCCEED = "[%1] 清空顶点成功: 共 %2 个顶点";

const QString INFO_CLOSE_POLYGON_SUCCEED = "[%1] 闭合多边形成功: 共 %2 个顶点";
const QString INFO_CLOSE_POLYGON_FAIL_1 = "[%1] 闭合多边形失败: 至少需 3 个顶点";
const QString INFO_CLOSE_POLYGON_FAIL_2 = "[%1] 闭合多边形失败: 将造成交叉线段\n[(%2,%3)(%4,%5)]×[(%6,%7)(%8,%9)]";
const QString INFO_CLOSE_POLYGON_FAIL_3 = "[%1] 闭合多边形失败: 内环不能包含内环";

const QString INFO_CLIP_SUCCEED = "[%1] 裁剪多边形完毕";

/***************
 * [枚举]
 ***************/
enum class OpStatus {
    waiting = 0,    // 等待中
    doing = 1,      // 绘制中
};


enum class PiStatus {
    illegal = 0,    // 非法的
    legal = 1,      // 合法的
};


/***************
 * [结构]
 ***************/
struct Point {
    // *** 变量 ***
    int x;      // X 横坐标
    int y;      // Y 纵坐标
    // *** 构造函数 ***
    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) { }
    // *** 运算符重载 ***
    bool operator ==(const Point& p) { return this->x == p.x && this->y == p.y; }
};


struct CPoint {
    // *** 变量 ***
    double x;       // X 横坐标
    double y;       // Y 纵坐标
    double t;       // t 参数 [0,1]
    CPoint *link;   // 顶点对应副本交点指针 nullptr: 非首位节点
    CPoint *other;  // 交点对应副本交点指针 nullptr: 非交点
    CPoint *next;   // 列表下一元素指针
    bool isEntry;   // True: 入点  False: 出点
    bool isVisited; // True: 已追踪  False: 未追踪
    // *** 构造函数 ***
    CPoint() : x(0), y(0), t(0), link(nullptr), other(nullptr), next(nullptr), isEntry(false), isVisited(false) { }
    CPoint(double x, double y, double t = 0, CPoint *l = nullptr, CPoint *o = nullptr, CPoint *n = nullptr, bool e = false, bool v = false)
        : x(x), y(y), t(t), link(l), other(o), next(n), isEntry(e), isVisited(v) { }
};


struct Line {
    // *** 变量 ***
    Point begin;            // 起点
    Point end;              // 终点
    CPoint *cpointList;     // 交点列表
    bool isEndLine;         // 是否为环的终边
    // *** 构造函数 ***
    Line() : begin(Point()), end(Point()), cpointList(nullptr), isEndLine(false)  { }
    Line(Point begin, Point end) : begin(begin), end(end), cpointList(nullptr), isEndLine(false) { }
    // *** 析构函数 ***
    ~Line() { CPoint *q = nullptr, *p = cpointList; while (p) { q = p; p = p->next; delete q; } }
    // *** 插入交点 ***
    void insertCPoint(CPoint *cpoint) {
        if (cpoint) {
            CPoint *q = nullptr, *p = cpointList;
            while (p && cpoint->t > p->t) { q = p; p = p->next; }
            if (q) { q->next = cpoint; cpoint->next = p; }
            else { cpointList = cpoint; cpoint->next = p; }
        }
    }
};

typedef  std::vector<Point> Polygon;

typedef  std::vector<std::vector<Point>> Polygons;

#endif // DEFINE_H
