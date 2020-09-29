#ifndef WINDOW_H
#define WINDOW_H

#include <QDebug>
#include <QEvent>
#include <QMainWindow>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPixmap>


#include "clipper.h"



QT_BEGIN_NAMESPACE
namespace Ui { class Window; }
QT_END_NAMESPACE

class Window : public QMainWindow
{
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

    Ui::Window *ui;     // UI 实例
    Clipper *clipper;   // 裁剪类实例

    // 图片资源
    QPixmap *pixmapALive;
    QPixmap *pixmapBLive;
    QPixmap *pixmapAWait;
    QPixmap *pixmapBWait;
    QPixmap *pixmapIllegal;
    QPixmap *pixmapLegal;
};
#endif // WINDOW_H
