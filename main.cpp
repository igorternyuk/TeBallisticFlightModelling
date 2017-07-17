#include "widget.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("fusion");
    Widget w;
    QRect screen = QApplication::desktop()->geometry();
    int dx = (screen.width() - w.width()) / 2;
    int dy = (screen.height() - w.height()) / 2;
    w.move(dx, dy);
    w.show();

    return a.exec();
}
