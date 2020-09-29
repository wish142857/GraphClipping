#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QEvent>
#include <QMouseEvent>

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

protected:
    void mousePressEvent(QMouseEvent *event);   // 鼠标点击事件处理函数

private:
    Ui::Window *ui;     // UI 实例
    Clipper *clipper;   // 裁剪类实例


};
#endif // WINDOW_H
