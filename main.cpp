#include "window.h"
#include "define.h"

#include <QApplication>
#include <QString>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Window w;

    // *** 设置窗口 ***
    w.setWindowTitle(QString::fromStdString(WINDOW_TITLE));
    w.setFixedWidth(WINDOW_WIDTH);
    w.setFixedHeight(WINDOW_HEIGHT);
    // *** 显示窗口 ***
    w.show();
    return a.exec();
}
