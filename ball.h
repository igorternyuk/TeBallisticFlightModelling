#ifndef BALL_H
#define BALL_H

#include <QGraphicsItem>

class Ball : public QGraphicsItem
{
public:
    explicit Ball(double x, double y, double r, QGraphicsItem *parent = nullptr);
    double x() const;
    double y() const;
    double r() const;
    void setY(double y);
    void setX(double x);
    bool isMoving() const;
    void setIsMoving(bool isMoving);

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
private:
    double m_x, m_y, m_r;
    bool m_isMoving = false;
};

#endif // BALL_H
