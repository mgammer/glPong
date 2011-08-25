#include <QtGui/QApplication>
#include "widget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget *w;

    if(argc > 1)
        w = new Widget(argv[1], 0);
    else
        w = new Widget();

    w->show();

    return a.exec();
}
