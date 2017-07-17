#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QButtonGroup>
class Scene;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    Scene * m_scene;
    QButtonGroup *m_radioButtonsGroup;
    enum screen_geom{WIDTH = 1200, HEIGHT = 650};
};

#endif // WIDGET_H
