#ifndef WINDOW_H
#define WINDOW_H

#include <QDebug>
#include <QEvent>
#include <QMainWindow>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPixmap>
#include <QTextBrowser>
#include <vector>
#include "clipper.h"



QT_BEGIN_NAMESPACE
namespace Ui { class Window; }
QT_END_NAMESPACE

class Window : public QMainWindow {
    Q_OBJECT

public:
    Window(QWidget *parent = nullptr);
    ~Window();

public slots:
    // 按钮点击事件处理函数
    void clickClipButton();
    void clickStartButtonA();
    void clickStartButtonB();
    void clickCancelButtonA();
    void clickCancelButtonB();
    void clickClearButtonA();
    void clickClearButtonB();

protected:
    void paintEvent(QPaintEvent *event);        // 绘制事件处理函数
    void mousePressEvent(QMouseEvent *event);   // 鼠标点击事件处理函数

private:
    OpStatus opStatusA; // A 操作状态
    OpStatus opStatusB; // B 操作状态
    PiStatus piStatusA; // A 图形状态
    PiStatus piStatusB; // B 图形状态

    int currentPolygonIndexA;   // A 当前多边形序号
    int currentPolygonIndexB;   // B 当前多边形序号
    std::vector<std::vector<Point>> polygonsA;   // A 多边形列表
    std::vector<std::vector<Point>> polygonsB;   // B 多边形列表

    Ui::Window *ui;     // UI 实例
    Clipper *clipper;   // 裁剪类实例

    void insertInfo(const QString &s);    // 添加通知信息函数
    void insertPointA(Point p);     // A 中插入新点函数
    void insertPointB(Point p);     // B 中插入新点函数
    void cancelPointA();            // A 中撤销上一点函数
    void cancelPointB();            // B 中撤销上一点函数
    void clearPointA();             // A 中清空所有点函数
    void clearPointB();             // B 中清空所有点函数
    void closePolygonA();           // A 中闭合多边形函数
    void closePolygonB();           // B 中闭合多边形函数
    void startClipAB();             // A、B 开始裁剪函数


    // 图片资源
    QPixmap *pixmapALive;
    QPixmap *pixmapBLive;
    QPixmap *pixmapAWait;
    QPixmap *pixmapBWait;
    QPixmap *pixmapIllegal;
    QPixmap *pixmapLegal;

};
#endif // WINDOW_H
