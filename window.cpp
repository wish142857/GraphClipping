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
    ui->centralwidget->setMouseTracking(true);
    this->setMouseTracking(true);
    ui->clipButton->setEnabled(false);
    // *** 变量初始化 ***
    opStatusA = OpStatus::doing;
    opStatusB = OpStatus::waiting;
    piStatusA = PiStatus::illegal;
    piStatusB = PiStatus::illegal;
    currentPolygonIndexA = currentPolygonIndexB = 0;
    polygonsA.push_back(Polygon());
    polygonsB.push_back(Polygon());
    // *** 画笔初始化 ***
    penA.setColor(QColor(0xd8, 0x1e, 0x06));
    penA.setWidth(DEFAULT_PEN_WIDTH);
    penA.setCapStyle(Qt::RoundCap);
    penB.setColor(QColor(0x12, 0x96, 0xdb));
    penB.setWidth(DEFAULT_PEN_WIDTH);
    penB.setCapStyle(Qt::RoundCap);
    penC.setColor(QColor(0xff, 0xd7, 0x00));
    penC.setWidth(DEFAULT_PEN_WIDTH);
    penC.setCapStyle(Qt::RoundCap);
    return;
}


Window::~Window() {
    delete ui;
    return;
}


/********************
 * [函数] 按钮点击事件处理函数
 ********************/
void Window::clickClipButton() { if (piStatusA == PiStatus::legal && piStatusB == PiStatus::legal) { startClipAB(); } update(); }

void Window::clickStartButtonA() { if (opStatusA == OpStatus::waiting) { opStatusA = OpStatus::doing; opStatusB = OpStatus::waiting; update(); } }

void Window::clickStartButtonB() { if (opStatusB == OpStatus::waiting) { opStatusB = OpStatus::doing; opStatusA = OpStatus::waiting; update(); } }

void Window::clickCancelButtonA() { if (opStatusA == OpStatus::doing) cancelPointA(); update(); }

void Window::clickCancelButtonB() { if (opStatusB == OpStatus::doing) cancelPointB(); update(); }

void Window::clickClearButtonA() { if (opStatusA == OpStatus::doing) clearPointA(); update(); }

void Window::clickClearButtonB() { if (opStatusB == OpStatus::doing) clearPointB(); update(); }


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
    ui->clipButton->setEnabled(piStatusA == PiStatus::legal && piStatusB == PiStatus::legal);
    // *** 更新绘图区图形 ***
    // * 初始化画笔 *
    QPainter painter(this);
    // * 绘制 A 图形 *
    painter.setPen(penA);
    for (int i = 0; i <= currentPolygonIndexA; i++) {
        for (int j = 0; j < int(polygonsA[i].size()); j++) {
            painter.drawPoint(QPoint(polygonsA[i][j].x - DRAWING_AREA_X_OFFSET, polygonsA[i][j].y - DRAWING_AREA_Y_OFFSET));
            if (j > 0)
                painter.drawLine(QPoint(polygonsA[i][j - 1].x - DRAWING_AREA_X_OFFSET, polygonsA[i][j - 1].y - DRAWING_AREA_Y_OFFSET),
                        QPoint(polygonsA[i][j].x - DRAWING_AREA_X_OFFSET, polygonsA[i][j].y - DRAWING_AREA_Y_OFFSET));
        }
    }
    for (int i = 0; i < currentPolygonIndexA; i++) {
        if (polygonsA[i].size() >= 3)
            painter.drawLine(QPoint(polygonsA[i][polygonsA[i].size() - 1].x - DRAWING_AREA_X_OFFSET, polygonsA[i][polygonsA[i].size() - 1].y - DRAWING_AREA_Y_OFFSET),
                    QPoint(polygonsA[i][0].x - DRAWING_AREA_X_OFFSET, polygonsA[i][0].y - DRAWING_AREA_Y_OFFSET));
    }
    // * 绘制 B 图形 *
    painter.setPen(penB);
    for (int i = 0; i <= currentPolygonIndexB; i++) {
        for (int j = 0; j < int(polygonsB[i].size()); j++) {
            painter.drawPoint(QPoint(polygonsB[i][j].x - DRAWING_AREA_X_OFFSET, polygonsB[i][j].y - DRAWING_AREA_Y_OFFSET));
            if (j > 0)
                painter.drawLine(QPoint(polygonsB[i][j - 1].x - DRAWING_AREA_X_OFFSET, polygonsB[i][j - 1].y - DRAWING_AREA_Y_OFFSET),
                        QPoint(polygonsB[i][j].x - DRAWING_AREA_X_OFFSET, polygonsB[i][j].y - DRAWING_AREA_Y_OFFSET));
        }
    }
    for (int i = 0; i < currentPolygonIndexB; i++) {
        if (polygonsB[i].size() >= 3)
            painter.drawLine(QPoint(polygonsB[i][polygonsB[i].size() - 1].x - DRAWING_AREA_X_OFFSET, polygonsB[i][polygonsB[i].size() - 1].y - DRAWING_AREA_Y_OFFSET),
                    QPoint(polygonsB[i][0].x - DRAWING_AREA_X_OFFSET, polygonsB[i][0].y - DRAWING_AREA_Y_OFFSET));
    }
    // * 绘制 C 图形 *
    painter.setPen(penC);
    for (Polygon &polygon : polygonsC) {
        for (int i = 0, j = polygon.size() - 1; i < int(polygon.size()); j = i++) {
            painter.drawPoint(QPoint(polygon[i].x - DRAWING_AREA_X_OFFSET, polygon[i].y - DRAWING_AREA_Y_OFFSET));
            painter.drawLine(QPoint(polygon[j].x - DRAWING_AREA_X_OFFSET, polygon[j].y - DRAWING_AREA_Y_OFFSET), QPoint(polygon[i].x - DRAWING_AREA_X_OFFSET, polygon[i].y - DRAWING_AREA_Y_OFFSET));
        }
    }
    return;
}


/********************
 * [函数] 鼠标移动事件处理函数
 ********************/
void Window::mouseMoveEvent(QMouseEvent *event) {
    int x = event->x() + DRAWING_AREA_X_OFFSET, y = event->y() + DRAWING_AREA_Y_OFFSET;
    if (x >= 0 && x < DRAWING_AREA_SIZE && y >= 0 && y < DRAWING_AREA_SIZE)
        ui->posLabel->setText(POS_LABEL_TEXT.arg(x).arg(y));
    else
        ui->posLabel->setText(POS_LABEL_TEXT.arg("?").arg("?"));
    update();
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
            if (opStatusA == OpStatus::doing)
                insertPointA(Point(x, y));
            else if (opStatusB == OpStatus::doing)
                insertPointB(Point(x, y));
            update();
        }
    }
    if (event->button() & Qt::RightButton) {
        // * 点击右键 *
        double x = DRAWING_AREA_X_OFFSET + event->x();
        double y = DRAWING_AREA_Y_OFFSET + event->y();
        if (x >= 0 && x < DRAWING_AREA_SIZE && y >= 0 && y < DRAWING_AREA_SIZE) {
            if (opStatusA == OpStatus::doing)
                closePolygonA();
            else if (opStatusB == OpStatus::doing)
                closePolygonB();
            update();
        }
    }
    return;
}


/********************
 * [函数] 添加通知信息函数
 ********************/
void Window::insertInfo(const QString &s, const QString &color) {
    if (ui->infoBrowser->document()->blockCount() >= INFO_MAX_LINE_NUMBER)
        ui->infoBrowser->clear();
    ui->infoBrowser->moveCursor(QTextCursor::End);
    ui->infoBrowser->append(QString::fromStdString("<font color=\"%1\">%2</font>\n").arg(color).arg(s));
    ui->infoBrowser->moveCursor(QTextCursor::End);
    return;
}


/********************
 * [函数] A 中插入新点函数
 ********************/
void Window::insertPointA(Point p) {
    // *** 验证合法性 ***
    // 验证重复顶点
    for (Polygon &polygon : polygonsA)
        for (Point &point : polygon)
            if (point == p) {
                insertInfo(INFO_INSERT_POINT_FAIL_1.arg("A").arg(p.x).arg(p.y), ERROR_COLOR);
                return;
            }
    if (currentPolygonIndexA == 0) {
        // * 绘制外图 *
        // 验证线段相交
        if (polygonsA[currentPolygonIndexA].size() > 0) {
            Polygon &polygon = polygonsA[0];
            const Line newLine = Line(polygon[polygon.size() - 1], p);
            for (int i = 0; i < int(polygon.size()) - 1; i++)
                if (checkLineWithLine(Line(polygon[i], polygon[i + 1]), newLine)) {
                    insertInfo(INFO_INSERT_POINT_FAIL_2.arg("A")
                               .arg(newLine.begin.x).arg(newLine.begin.y).arg(newLine.end.x).arg(newLine.end.y)
                               .arg(polygon[i].x).arg(polygon[i].y).arg(polygon[i + 1].x).arg(polygon[i + 1].y),
                               ERROR_COLOR);
                    return;
                }
        }
    } else {
        // * 绘制内环 *
        // 验证顶点是否在基本矩形内
        if (!checkPointInPolygon(p, polygonsA[0])) {
            insertInfo(INFO_INSERT_POINT_FAIL_3.arg("A").arg(p.x).arg(p.y), ERROR_COLOR);
            return;
        }
        // 验证顶点是否在其他内环内
        for (int k = 1; k < currentPolygonIndexA; k++)
            if (checkPointInPolygon(p, polygonsA[k]))   {
                insertInfo(INFO_INSERT_POINT_FAIL_4.arg("A").arg(p.x).arg(p.y), ERROR_COLOR);
                return;
            }
        // 验证线段相交
        if (polygonsA[currentPolygonIndexA].size() > 0) {
            const Line newLine = Line(polygonsA[currentPolygonIndexA][polygonsA[currentPolygonIndexA].size() - 1], p);
            for (int k = 1; k <= currentPolygonIndexA; k++) {
                Polygon &polygon = polygonsA[k];
                for (int i = 0; i < int(polygon.size()) - 1; i++)
                    if (checkLineWithLine(Line(polygon[i], polygon[i + 1]), newLine)) {
                        insertInfo(INFO_INSERT_POINT_FAIL_2.arg("A")
                                   .arg(newLine.begin.x).arg(newLine.begin.y).arg(newLine.end.x).arg(newLine.end.y)
                                   .arg(polygon[i].x).arg(polygon[i].y).arg(polygon[i + 1].x).arg(polygon[i + 1].y),
                                   ERROR_COLOR);
                        return;
                    }
                if (k != currentPolygonIndexA && checkLineWithLine(Line(polygon[polygon.size() - 1], polygon[0]), newLine)) {
                    insertInfo(INFO_INSERT_POINT_FAIL_2.arg("A")
                               .arg(newLine.begin.x).arg(newLine.begin.y).arg(newLine.end.x).arg(newLine.end.y)
                               .arg(polygon[polygon.size() - 1].x).arg(polygon[polygon.size() - 1].y).arg(polygon[0].x).arg(polygon[0].y),
                               ERROR_COLOR);
                    return;
                }
            }
        }
    }
    // *** 开始插入 ***
    polygonsA[currentPolygonIndexA].push_back(p);
    piStatusA = PiStatus::illegal; // 更新图形状态 - 非法
    polygonsC.clear();
    insertInfo(INFO_INSERT_POINT_SUCCEED.arg("A").arg(p.x).arg(p.y), SUCCESS_COLOR);
    return;
}


/********************
 * [函数] B 中插入新点函数
 ********************/
void Window::insertPointB(Point p) {
    // *** 验证合法性 ***
    // 验证重复顶点
    for (Polygon &polygon : polygonsB)
        for (Point &point : polygon)
            if (point == p) {
                insertInfo(INFO_INSERT_POINT_FAIL_1.arg("B").arg(p.x).arg(p.y), ERROR_COLOR);
                return;
            }
    if (currentPolygonIndexB == 0) {
        // * 绘制外图 *
        // 验证线段相交
        if (polygonsB[currentPolygonIndexB].size() > 0) {
            Polygon &polygon = polygonsB[0];
            const Line newLine = Line(polygon[polygon.size() - 1], p);
            for (int i = 0; i < int(polygon.size()) - 1; i++)
                if (checkLineWithLine(Line(polygon[i], polygon[i + 1]), newLine)) {
                    insertInfo(INFO_INSERT_POINT_FAIL_2.arg("B")
                               .arg(newLine.begin.x).arg(newLine.begin.y).arg(newLine.end.x).arg(newLine.end.y)
                               .arg(polygon[i].x).arg(polygon[i].y).arg(polygon[i + 1].x).arg(polygon[i + 1].y),
                               ERROR_COLOR);
                    return;
                }
        }
    } else {
        // * 绘制内环 *
        // 验证顶点是否在基本矩形内
        if (!checkPointInPolygon(p, polygonsB[0])) {
            insertInfo(INFO_INSERT_POINT_FAIL_3.arg("B").arg(p.x).arg(p.y), ERROR_COLOR);
            return;
        }
        // 验证顶点是否在其他内环内
        for (int k = 1; k < currentPolygonIndexB; k++)
            if (checkPointInPolygon(p, polygonsB[k]))   {
                insertInfo(INFO_INSERT_POINT_FAIL_4.arg("B").arg(p.x).arg(p.y), ERROR_COLOR);
                return;
            }
        // 验证线段相交
        if (polygonsB[currentPolygonIndexB].size() > 0) {
            const Line newLine = Line(polygonsB[currentPolygonIndexB][polygonsB[currentPolygonIndexB].size() - 1], p);
            for (int k = 1; k <= currentPolygonIndexB; k++) {
                Polygon &polygon = polygonsB[k];
                for (int i = 0; i < int(polygon.size()) - 1; i++)
                    if (checkLineWithLine(Line(polygon[i], polygon[i + 1]), newLine)) {
                        insertInfo(INFO_INSERT_POINT_FAIL_2.arg("B")
                                   .arg(newLine.begin.x).arg(newLine.begin.y).arg(newLine.end.x).arg(newLine.end.y)
                                   .arg(polygon[i].x).arg(polygon[i].y).arg(polygon[i + 1].x).arg(polygon[i + 1].y),
                                   ERROR_COLOR);
                        return;
                    }
                if (k != currentPolygonIndexB && checkLineWithLine(Line(polygon[polygon.size() - 1], polygon[0]), newLine)) {
                    insertInfo(INFO_INSERT_POINT_FAIL_2.arg("B")
                               .arg(newLine.begin.x).arg(newLine.begin.y).arg(newLine.end.x).arg(newLine.end.y)
                               .arg(polygon[polygon.size() - 1].x).arg(polygon[polygon.size() - 1].y).arg(polygon[0].x).arg(polygon[0].y),
                               ERROR_COLOR);
                    return;
                }
            }
        }
    }
    // *** 开始插入 ***
    polygonsB[currentPolygonIndexB].push_back(p);
    piStatusB = PiStatus::illegal; // 更新图形状态 - 非法
    polygonsC.clear();
    insertInfo(INFO_INSERT_POINT_SUCCEED.arg("B").arg(p.x).arg(p.y), SUCCESS_COLOR);
    return;
}


/********************
 * [函数] A 中撤销上一点
 ********************/
void Window::cancelPointA() {
    // *** 验证合法性 ***
    // 保证之前的绘制均为合法，则撤销时无需验证
    while (currentPolygonIndexA >= 0 && polygonsA[currentPolygonIndexA].empty())
        currentPolygonIndexA--;
    if (currentPolygonIndexA < 0) {
        currentPolygonIndexA = 0;
        insertInfo(INFO_CANCEL_FAIL.arg("A"), ERROR_COLOR);
    } else {
        // *** 开始撤销 ***
        Point p = polygonsA[currentPolygonIndexA].back();
        polygonsA[currentPolygonIndexA].pop_back();
        if (currentPolygonIndexA != 0 && polygonsA[currentPolygonIndexA].size() == 0)
            piStatusA = PiStatus::legal; // 更新图形状态 - 合法
        else
            piStatusA = PiStatus::illegal; // 更新图形状态 - 非法
        polygonsC.clear();
        insertInfo(INFO_CANCEL_SUCCEED.arg("A").arg(p.x).arg(p.y), SUCCESS_COLOR);
    }
    return;
}


/********************
 * [函数] B 中撤销上一点
 ********************/
void Window::cancelPointB() {
    // *** 验证合法性 ***
    // 保证之前的绘制均为合法，则撤销时无需验证
    while (currentPolygonIndexB >= 0 && polygonsB[currentPolygonIndexB].empty())
        currentPolygonIndexB--;
    if (currentPolygonIndexB < 0) {
        currentPolygonIndexB = 0;
        insertInfo(INFO_CANCEL_FAIL.arg("B"), ERROR_COLOR);
    } else {
        // *** 开始撤销 ***
        Point p = polygonsB[currentPolygonIndexB].back();
        polygonsB[currentPolygonIndexB].pop_back();
        if (currentPolygonIndexB != 0 && polygonsB[currentPolygonIndexB].size() == 0)
            piStatusB = PiStatus::legal; // 更新图形状态 - 合法
        else
            piStatusB = PiStatus::illegal; // 更新图形状态 - 非法
        polygonsC.clear();
        insertInfo(INFO_CANCEL_SUCCEED.arg("B").arg(p.x).arg(p.y), SUCCESS_COLOR);
    }
    return;
}


/********************
 * [函数] A 中清空所有点函数
 ********************/
void Window::clearPointA() {
    // *** 开始清空 ***
    int pointNumber = 0;
    for (Polygon &polygon : polygonsA)
        pointNumber += polygon.size();
    currentPolygonIndexA = 0;
    polygonsA.clear();
    polygonsA.push_back(Polygon());
    piStatusA = PiStatus::illegal; // 更新图形状态 - 非法
    polygonsC.clear();
    insertInfo(INFO_CLEAR_SUCCEED.arg("A").arg(pointNumber), SUCCESS_COLOR);
    return;
}


/********************
 * [函数] B 中清空所有点函数
 ********************/
void Window::clearPointB() {
    // *** 开始清空 ***
    int pointNumber = 0;
    for (Polygon &polygon : polygonsB)
        pointNumber += polygon.size();
    currentPolygonIndexB = 0;
    polygonsB.clear();
    polygonsB.push_back(Polygon());
    piStatusB = PiStatus::illegal; // 更新图形状态 - 非法
    polygonsC.clear();
    insertInfo(INFO_CLEAR_SUCCEED.arg("B").arg(pointNumber), SUCCESS_COLOR);
    return;
}


/********************
 * [函数] A 中闭合多边形
 ********************/
void Window::closePolygonA() {
    // *** 验证合法性 ***
    // 是否有至少 3 个顶点
    int pointNumber = polygonsA[currentPolygonIndexA].size();
    if (pointNumber < 3) {
        insertInfo(INFO_CLOSE_POLYGON_FAIL_1.arg("A"), ERROR_COLOR);
        return;
    }
    // *** 验证合法性 ***
    const Line newLine = Line(polygonsA[currentPolygonIndexA][pointNumber - 1], polygonsA[currentPolygonIndexA][0]);
    if (currentPolygonIndexA == 0) {
        // * 闭合外图 *
        // 闭合线段是否与其他线段规范相交
        Polygon &polygon = polygonsA[0];
        for (int i = 0; i < int(polygon.size()) - 1; i++)
            if (checkLineWithLine(Line(polygon[i], polygon[i + 1]), newLine)) {
                insertInfo(INFO_CLOSE_POLYGON_FAIL_2.arg("A"), ERROR_COLOR);
                return;
            }
    } else {
        // * 闭合内环 *
        // 闭合线段是否与其他线段规范相交
        for (int k = 1; k <= currentPolygonIndexA; k++) {
            Polygon &polygon = polygonsA[k];
            for (int i = 0; i < int(polygon.size()) - 1; i++)
                if (checkLineWithLine(Line(polygon[i], polygon[i + 1]), newLine)) {
                    insertInfo(INFO_CLOSE_POLYGON_FAIL_2.arg("A"), ERROR_COLOR);
                    return;
                }
            if (k != currentPolygonIndexA && checkLineWithLine(Line(polygon[polygon.size() - 1], polygon[0]), newLine)) {
                insertInfo(INFO_CLOSE_POLYGON_FAIL_2.arg("A"), ERROR_COLOR);
                return;
            }
        }
        // 闭合内环是否包含其他内环
        for (int k = 1; k < currentPolygonIndexA; k++)
            if (checkPointInPolygon(polygonsA[k].front(), polygonsA[currentPolygonIndexA]) || checkPointInPolygon(polygonsA[k].back(), polygonsA[currentPolygonIndexA])) {
                insertInfo(INFO_CLOSE_POLYGON_FAIL_3.arg("A"), ERROR_COLOR);
                return;
            }
    }
    // *** 开始闭合 ***
    currentPolygonIndexA++;
    polygonsA.push_back(Polygon());
    piStatusA = PiStatus::legal; // 更新图形状态 - 合法
    insertInfo(INFO_CLOSE_POLYGON_SUCCEED.arg("A").arg(pointNumber), SUCCESS_COLOR);
    return;
}


/********************
 * [函数] B 中闭合多边形
 ********************/
void Window::closePolygonB() {
    // *** 验证合法性 ***
    // 是否有至少 3 个顶点
    int pointNumber = polygonsB[currentPolygonIndexB].size();
    if (pointNumber < 3) {
        insertInfo(INFO_CLOSE_POLYGON_FAIL_1.arg("B"), ERROR_COLOR);
        return;
    }
    // *** 验证正确性 ***
    const Line newLine = Line(polygonsB[currentPolygonIndexB][pointNumber - 1], polygonsB[currentPolygonIndexB][0]);
    if (currentPolygonIndexB == 0) {
        // * 闭合外图 *
        // 闭合线段是否与其他线段规范相交
        Polygon &polygon = polygonsB[0];
        for (int i = 0; i < int(polygon.size()) - 1; i++)
            if (checkLineWithLine(Line(polygon[i], polygon[i + 1]), newLine)) {
                insertInfo(INFO_CLOSE_POLYGON_FAIL_2.arg("B"), ERROR_COLOR);
                return;
            }
    } else {
        // * 闭合内环 *
        // 闭合线段是否与其他线段规范相交
        for (int k = 1; k <= currentPolygonIndexB; k++) {
            Polygon &polygon = polygonsB[k];
            for (int i = 0; i < int(polygon.size()) - 1; i++)
                if (checkLineWithLine(Line(polygon[i], polygon[i + 1]), newLine)) {
                    insertInfo(INFO_CLOSE_POLYGON_FAIL_2.arg("B"), ERROR_COLOR);
                    return;
                }
            if (k != currentPolygonIndexB && checkLineWithLine(Line(polygon[polygon.size() - 1], polygon[0]), newLine)) {
                insertInfo(INFO_CLOSE_POLYGON_FAIL_2.arg("B"), ERROR_COLOR);
                return;
            }
        }
        // 闭合内环是否包含其他内环
        for (int k = 1; k < currentPolygonIndexB; k++)
            if (checkPointInPolygon(polygonsB[k].front(), polygonsB[currentPolygonIndexB]) || checkPointInPolygon(polygonsB[k].back(), polygonsB[currentPolygonIndexB])) {
                insertInfo(INFO_CLOSE_POLYGON_FAIL_3.arg("B"), ERROR_COLOR);
                return;
            }
    }
    // *** 开始闭合 ***
    currentPolygonIndexB++;
    polygonsB.push_back(Polygon());
    piStatusB = PiStatus::legal; // 更新图形状态 - 合法
    insertInfo(INFO_CLOSE_POLYGON_SUCCEED.arg("B").arg(pointNumber), SUCCESS_COLOR);
    return;
}


/********************
 * [函数] A、B 开始裁剪函数
 ********************/
void Window::startClipAB() {
    // TODO
    startClipPolygon(polygonsA, polygonsB, polygonsC);
    // insertInfo()
    return;
}
