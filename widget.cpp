#include "widget.h"
#include "ui_widget.h"
#include <QResizeEvent>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    glWidget = new glCoreWidget(this);
    glWidget->setGeometry(0, 35, 400, 200);

    connect(this, SIGNAL(gameStart(bool)), glWidget, SLOT(startGame(bool)));
    connect(glWidget, SIGNAL(goal(int)), this, SLOT(increaseLCD(int)));
    connect(glWidget, SIGNAL(resetLCD()), this, SLOT(resetLCD()));
    connect(glWidget, SIGNAL(toggleFullscreen()), this, SLOT(toggleFullscreen()));

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(buttonPressed()));
    connect(glWidget, SIGNAL(toggleGame()), this, SLOT(buttonPressed()));

    glWidget->setFocus();

}

Widget::~Widget()
{
    delete ui;
    if(glWidget)
        delete glWidget;
}

void Widget::resizeEvent(QResizeEvent *e)
{
    ui->lcdNumber->setGeometry(20, 10, 64, 23);
    ui->lcdNumber_2->setGeometry(e->size().width() - 20 - 64, 10, 64, 23);
    ui->pushButton->setGeometry(e->size().width()/2 - 42, e->size().height() - 30, 84, 26);

    glWidget->setGeometry(0, 35, e->size().width(), e->size().width()/2);
}

void Widget::buttonPressed()
{
    if(ui->pushButton->text().toLower() == QString("Start").toLower()) {
        ui->pushButton->setText("Pause");
        emit gameStart(true);
    }
    else if(ui->pushButton->text().toLower() == QString("Pause").toLower()) {
        ui->pushButton->setText("Start");
        emit gameStart(false);
    }
    glWidget->setFocus();       // set focus to glWidget otherwise we dont get keyEvents on glWidget
}

void Widget::increaseLCD(int player)
{
    if(player == 1)
        ui->lcdNumber->display(ui->lcdNumber->intValue()+1);
    else
        ui->lcdNumber_2->display(ui->lcdNumber_2->intValue()+1);
}

void Widget::resetLCD()
{
    ui->lcdNumber->display(0);
    ui->lcdNumber_2->display(0);
}

void Widget::toggleFullscreen()
{
    if(this->isFullScreen())
        this->showNormal();
    else
        this->showFullScreen();
}
