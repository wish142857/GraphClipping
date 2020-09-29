#include "window.h"
#include "ui_window.h"
#include <QDebug>

Window::Window(QWidget *parent) : QMainWindow(parent), ui(new Ui::Window)
{
    ui->setupUi(this);
}

Window::~Window()
{
    delete ui;
}


/********************
 * [函数] 鼠标点击事件处理函数
 ********************/
void Window::mousePressEvent(QMouseEvent *event)
{
    if (event->button() & Qt::LeftButton)
    {
        // * 点击左键 *
        int x = DRAWING_AREA_X_OFFSET + event->x();
        int y = DRAWING_AREA_Y_OFFSET + event->y();
        if (x >= 0 && x < DRAWING_AREA_SIZE && y >= 0 && y < DRAWING_AREA_SIZE) {
            qDebug() << x << "  " << y << Qt::endl;
        }
    }
    if (event->button() & Qt::RightButton)
    {
        // * 点击右键 *
        int x = DRAWING_AREA_X_OFFSET + event->x();
        int y = DRAWING_AREA_Y_OFFSET + event->y();
        if (x >= 0 && x < DRAWING_AREA_SIZE && y >= 0 && y < DRAWING_AREA_SIZE) {
            qDebug() << x << "  " << y << Qt::endl;
        }
    }
    return;
}



