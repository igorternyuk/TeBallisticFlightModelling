#include "ball.h"
#include <QPainter>

Ball::Ball(double x, double y, double r, QGraphicsItem *parent):
    QGraphicsItem(parent), m_x(x), m_y(y), m_r(r)
{}

double Ball::x() const
{
    return m_x;
}

double Ball::y() const
{
    return m_y;
}

void Ball::setY(double y)
{
    m_y = y;
}

void Ball::setX(double x)
{
    m_x = x;
}


double Ball::r() const
{
    return m_r;
}

QRectF Ball::boundingRect() const
{
    return QRectF(m_x - m_r, m_y - m_r, 2 * m_r, 2 * m_r);
}

void Ball::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    (void)option;
    (void)widget;
    if(this->isMoving())
    {
        painter->setBrush(Qt::blue);
        painter->drawEllipse(QPointF(m_x,m_y), m_r, m_r);
    }
    painter->setBrush(Qt::NoBrush);
}

bool Ball::isMoving() const
{
    return m_isMoving;
}

void Ball::setIsMoving(bool isMoving)
{
    m_isMoving = isMoving;
}

