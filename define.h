#ifndef DEFINE_H
#define DEFINE_H
#include <vector>
#include <string>
#include <QString>

/***************
 * [常量]
 ***************/
const std::string WINDOW_TITLE = "『 iClip 』 -  Ver 0.1"; // 窗口标题
const int WINDOW_WIDTH = 1200;              // 窗口宽度
const int WINDOW_HEIGHT = 900;              // 窗口高度
const int DRAWING_AREA_SIZE = 900;          // 绘图区域边长
const int DRAWING_AREA_X_OFFSET = -300;     // 绘图区域横轴偏移值
const int DRAWING_AREA_Y_OFFSET = 0;        // 绘图区域纵轴偏移值
const int INFO_MAX_LINE_NUMBER = 100;       // 信息最大行数
const int DEFAULT_PEN_WIDTH = 3;            // 默认画笔宽度

const QString START_BUTTON_TEXT_1 = "开始绘图";
const QString START_BUTTON_TEXT_2 = "正在绘图";

const QString INFO_INSERT_POINT_SUCCEED = "[%1] 插入顶点成功: (%2, %3)";

const QString INFO_CANCEL_SUCCEED = "[%1] 撤销顶点成功: (%2, %3)";
const QString INFO_CANCEL_FAIL = "[%1] 撤销顶点失败: 已无剩余顶点";

const QString INFO_CLEAR_SUCCEED = "[%1] 清空顶点成功: 共 %2 个顶点";

const QString INFO_CLOSE_POLYGON_SUCCEED = "[%1] 闭合多边形成功: 共 %2 个顶点";
const QString INFO_CLOSE_POLYGON_FAIL_1 = "[%1] 闭合多边形失败: 至少需 3 个顶点";
const QString INFO_CLOSE_POLYGON_FAIL_2 = "[%1] 闭合多边形失败: 将造成交叉线段";




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
    int x;
    int y;

    Point() : x(0), y(0) {}
    Point(int x) : x(x), y(x) {}
    Point(int x, int y) : x(x), y(y) { }
};

struct Line {
    Point begin;
    Point end;

    Line() : begin(Point()), end(Point()) { }
    Line(Point begin, Point end) : begin(begin), end(end) { }
};


typedef  std::vector<Point> Polygon;

typedef  std::vector<std::vector<Point>> Polygons;

#endif // DEFINE_H
