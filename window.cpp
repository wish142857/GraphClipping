#include "window.h"
#include "ui_window.h"


Window::Window(QWidget *parent) : QMainWindow(parent), ui(new Ui::Window)
{
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
    // *** 变量初始化 ***
    opStatusA = OpStatus::doing;
    opStatusB = OpStatus::waiting;

    piStatusA = PiStatus::illegal;
    piStatusB = PiStatus::illegal;

    return;
}


Window::~Window()
{
    delete ui;
    return;
}


/********************
 * [函数] 按钮点击事件处理函数
 ********************/
void Window::clickClipButton()
{
    return;
}


void Window::clickStartButtonA()
{
    return;
}


void Window::clickStartButtonB()
{
    return;
}


void Window::clickCancelButtonA()
{
    return;
}


void Window::clickCancelButtonB()
{
    return;
}


void Window::clickClearButtonA()
{
    return;
}


void Window::clickClearButtonB()
{
    return;
}


/********************
 * [函数] 绘制事件处理函数
 ********************/
void Window::paintEvent(QPaintEvent *event)
{
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
    if (opStatusA == OpStatus::doing)
    {
        ui->startButton_A->setText(QString::fromStdString(START_BUTTON_TEXT_2));
        ui->cancelButton_A->setEnabled(true);
        ui->clearButton_A->setEnabled(true);

    }
    else
    {
        ui->startButton_A->setText(QString::fromStdString(START_BUTTON_TEXT_1));
        ui->cancelButton_A->setEnabled(false);
        ui->clearButton_A->setEnabled(false);
    }


    return;
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



