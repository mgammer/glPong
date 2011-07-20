#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include "glcorewidget.h"

namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

signals:
    void gameStart(bool state);

public slots:
    void increaseLCD(int player);
    void buttonPressed();

private slots:
    void resetLCD();
    void toggleFullscreen();



protected:
    void resizeEvent(QResizeEvent *e);

private:
    Ui::Widget *ui;

    glCoreWidget *glWidget;
};

#endif // WIDGET_H
