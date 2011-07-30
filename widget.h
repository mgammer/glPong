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
    void sliderValue(int value);
    void checkBoxValue(bool state);

public slots:

private slots:
    void resetLCD();
    void increaseLCD(int player);
    void buttonPressed();
    void toggleFullscreen();
    void sliderValueChanged(int value);
    void checkBoxValueChanged(bool state);

protected:
    void resizeEvent(QResizeEvent *e);

private:
    Ui::Widget *ui;

    glCoreWidget *glWidget;
};

#endif // WIDGET_H
