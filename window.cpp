#include "window.h"
#include "ui_window.h"


Window::Window(QWidget *parent) : QMainWindow(parent), ui(new Ui::Window) {
    // *** 加载 UI ***
    ui->setupUi(this);
    // *** 加载资源 ***
    pixmapALive = new QPixmap(":/logo/res/A#d81e06.png");
    pixmapBLive = new QPixmap(":/logo/res/B#1296db.png");
    pixmapAWait = new QPixmap(":/logo/res/A#cccccc.png");
    pixmapBWait = new QPixmap(":/logo/res/B#cccccc.png");
    pixmapIllegal = new QPixmap(":/logo/res/N#d81e06.png");
    pixmapLegal = new QPixmap(":/logo/res/P#1afa29.png");
    // *** 关联信号槽 ***
    connect(ui->clipButton, SIGNAL(clicked()), this, SLOT(clickClipButton()));
    connect(ui->startButton_A, SIGNAL(clicked()), this, SLOT(clickStartButtonA()));
    connect(ui->startButton_B, SIGNAL(clicked()), this, SLOT(clickStartButtonB()));
    connect(ui->cancelButton_A, SIGNAL(clicked()), this, SLOT(clickCancelButtonA()));
    connect(ui->cancelButton_B, SIGNAL(clicked()), this, SLOT(clickCancelButtonB()));
    connect(ui->clearButton_A, SIGNAL(clicked()), this, SLOT(clickClearButtonA()));
    connect(ui->clearButton_B, SIGNAL(clicked()), this, SLOT(clickClearButtonB()));
    // *** 组件初始化 ***
    ui->clipButton->setEnabled(false);
    // *** 变量初始化 ***
    clipper = new Clipper();
    opStatusA = OpStatus::doing;
    opStatusB = OpStatus::waiting;
    piStatusA = PiStatus::illegal;
    piStatusB = PiStatus::illegal;
    currentPolygonIndexA = currentPolygonIndexB = 0;
    polygonsA.push_back(std::vector<Point>());
    polygonsB.push_back(std::vector<Point>());
    return;
}


Window::~Window() {
    delete ui;
    delete clipper;
    return;
}


/********************
 * [函数] 按钮点击事件处理函数
 ********************/
void Window::clickClipButton() { if (piStatusA == PiStatus::legal && piStatusB == PiStatus::legal) { startClipAB(); } }

void Window::clickStartButtonA() { if (opStatusA == OpStatus::waiting) { opStatusA = OpStatus::doing; opStatusB = OpStatus::waiting; } }

void Window::clickStartButtonB() { if (opStatusB == OpStatus::waiting) { opStatusB = OpStatus::doing; opStatusA = OpStatus::waiting; } }

void Window::clickCancelButtonA() { if (opStatusA == OpStatus::doing) cancelPointA(); }

void Window::clickCancelButtonB() { if (opStatusB == OpStatus::doing) cancelPointB(); }

void Window::clickClearButtonA() { if (opStatusA == OpStatus::doing) clearPointA(); }

void Window::clickClearButtonB() { if (opStatusB == OpStatus::doing) clearPointB(); }


/********************
 * [函数] 绘制事件处理函数
 ********************/
void Window::paintEvent(QPaintEvent *event) {
    // *** 更新图标状态 ***
    if (opStatusA == OpStatus::doing)
        ui->logoLabel_A->setPixmap(*pixmapALive);
    else if (opStatusA == OpStatus::waiting)
        ui->logoLabel_A->setPixmap(*pixmapAWait);
    if (opStatusB == OpStatus::doing)
        ui->logoLabel_B->setPixmap(*pixmapBLive);
    else if (opStatusB == OpStatus::waiting)
        ui->logoLabel_B->setPixmap(*pixmapBWait);
    if (piStatusA == PiStatus::legal)
        ui->statusLabel_A->setPixmap(*pixmapLegal);
    else if (piStatusA == PiStatus::illegal)
        ui->statusLabel_A->setPixmap(*pixmapIllegal);
    if (piStatusB == PiStatus::legal)
        ui->statusLabel_B->setPixmap(*pixmapLegal);
    else if (piStatusB == PiStatus::illegal)
        ui->statusLabel_B->setPixmap(*pixmapIllegal);
    // *** 更新按钮状态 ***
    if (opStatusA == OpStatus::doing) {
        ui->startButton_A->setText(START_BUTTON_TEXT_2);
        ui->startButton_A->setEnabled(false);
        ui->cancelButton_A->setEnabled(true);
        ui->clearButton_A->setEnabled(true);

    } else {
        ui->startButton_A->setText(START_BUTTON_TEXT_1);
        ui->startButton_A->setEnabled(true);
        ui->cancelButton_A->setEnabled(false);
        ui->clearButton_A->setEnabled(false);
    }
    if (opStatusB == OpStatus::doing) {
        ui->startButton_B->setText(START_BUTTON_TEXT_2);
        ui->startButton_B->setEnabled(false);
        ui->cancelButton_B->setEnabled(true);
        ui->clearButton_B->setEnabled(true);

    } else {
        ui->startButton_B->setText(START_BUTTON_TEXT_1);
        ui->startButton_B->setEnabled(true);
        ui->cancelButton_B->setEnabled(false);
        ui->clearButton_B->setEnabled(false);
    }
    return;
}


/********************
 * [函数] 鼠标点击事件处理函数
 ********************/
void Window::mousePressEvent(QMouseEvent *event) {
    if (event->button() & Qt::LeftButton) {
        // * 点击左键 *
        int x = DRAWING_AREA_X_OFFSET + event->x();
        int y = DRAWING_AREA_Y_OFFSET + event->y();
        if (x >= 0 && x < DRAWING_AREA_SIZE && y >= 0 && y < DRAWING_AREA_SIZE) {
            this->insertInfo("AAA" + QString(x) + QString(y));
            qDebug() << x << "  " << y << Qt::endl;
        }
    }
    if (event->button() & Qt::RightButton) {
        // * 点击右键 *
        int x = DRAWING_AREA_X_OFFSET + event->x();
        int y = DRAWING_AREA_Y_OFFSET + event->y();
        if (x >= 0 && x < DRAWING_AREA_SIZE && y >= 0 && y < DRAWING_AREA_SIZE) {
            qDebug() << x << "  " << y << Qt::endl;
        }
    }
    return;
}


/********************
 * [函数] 添加通知信息函数
 ********************/
void Window::insertInfo(const QString &s) {
    if (ui->infoBrowser->document()->blockCount() >= INFO_MAX_LINE_NUMBER)
        ui->infoBrowser->clear();
    ui->infoBrowser->moveCursor(QTextCursor::End);
    ui->infoBrowser->insertPlainText(s);
    ui->infoBrowser->insertPlainText("\n");
    ui->infoBrowser->moveCursor(QTextCursor::End);
    return;
}


/********************
 * [函数] A 中插入新点函数
 ********************/
void Window::insertPointA(Point p) {
    polygonsA[currentPolygonIndexA].push_back(p);
    insertInfo(INFO_INSERT_POINT_SUCCEED.arg("A").arg(p.x).arg(p.y));
    // TODO 验证正确性
    return;
}


/********************
 * [函数] B 中插入新点函数
 ********************/
void Window::insertPointB(Point p) {
    polygonsB[currentPolygonIndexB].push_back(p);
    insertInfo(INFO_INSERT_POINT_SUCCEED.arg("B").arg(p.x).arg(p.y));
    // TODO 验证正确性
    return;
}


/********************
 * [函数] A 中撤销上一点
 ********************/
void Window::cancelPointA() {
    while (currentPolygonIndexA >= 0 && polygonsA[currentPolygonIndexA].empty())
        currentPolygonIndexA--;
    if (currentPolygonIndexA < 0) {
        currentPolygonIndexA = 0;
        insertInfo(INFO_CANCEL_FAIL.arg("A"));
    } else {
        Point p = polygonsA[currentPolygonIndexA].back();
        polygonsA[currentPolygonIndexA].pop_back();
        insertInfo(INFO_CANCEL_SUCCEED.arg("A").arg(p.x).arg(p.y));
    }
    // TODO 验证正确性
    return;
}


/********************
 * [函数] B 中撤销上一点
 ********************/
void Window::cancelPointB() {
    while (currentPolygonIndexB >= 0 && polygonsB[currentPolygonIndexB].empty())
        currentPolygonIndexB--;
    if (currentPolygonIndexB < 0) {
        currentPolygonIndexB = 0;
        insertInfo(INFO_CANCEL_FAIL.arg("B"));
    } else {
        Point p = polygonsB[currentPolygonIndexB].back();
        polygonsB[currentPolygonIndexB].pop_back();
        insertInfo(INFO_CANCEL_SUCCEED.arg("B").arg(p.x).arg(p.y));
    }
    // TODO 验证正确性
    return;
}


/********************
 * [函数] A 中清空所有点函数
 ********************/
void Window::clearPointA() {
    int pointNumber = 0;
    for (std::vector<Point> &polygon : polygonsA)
        pointNumber += polygon.size();
    currentPolygonIndexA = 0;
    polygonsA.clear();
    polygonsA.push_back(std::vector<Point>());
    piStatusA = PiStatus::illegal;
    insertInfo(INFO_CLEAR_SUCCEED.arg("A").arg(pointNumber));
    return;
}


/********************
 * [函数] B 中清空所有点函数
 ********************/
void Window::clearPointB() {
    int pointNumber = 0;
    for (std::vector<Point> &polygon : polygonsB)
        pointNumber += polygon.size();
    currentPolygonIndexB = 0;
    polygonsB.clear();
    polygonsB.push_back(std::vector<Point>());
    piStatusB = PiStatus::illegal;
    insertInfo(INFO_CLEAR_SUCCEED.arg("B").arg(pointNumber));
    return;
}


/********************
 * [函数] A 中闭合多边形
 ********************/
void Window::closePolygonA() {
    int pointNumber = polygonsA[currentPolygonIndexA].size();
    if (pointNumber < 3) {
        insertInfo(INFO_CLOSE_POLYGON_FAIL.arg("A"));
        return;
    }
    currentPolygonIndexA++;
    polygonsA.push_back(std::vector<Point>());
    insertInfo(INFO_CLOSE_POLYGON_SUCCEED.arg("A").arg(pointNumber));
    // TODO 验证正确性
    return;
}


/********************
 * [函数] B 中闭合多边形
 ********************/
void Window::closePolygonB() {
    int pointNumber = polygonsB[currentPolygonIndexB].size();
    if (pointNumber < 3) {
        insertInfo(INFO_CLOSE_POLYGON_FAIL.arg("B"));
        return;
    }
    currentPolygonIndexB++;
    polygonsB.push_back(std::vector<Point>());
    insertInfo(INFO_CLOSE_POLYGON_SUCCEED.arg("B").arg(pointNumber));
    // TODO 验证正确性
    return;
}


/********************
 * [函数] A、B 开始裁剪函数
 ********************/
void Window::startClipAB() {
    // TODO
}


