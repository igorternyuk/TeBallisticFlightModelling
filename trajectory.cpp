#include <QPainter>
#include <QPen>
#include "trajectory.h"
#ifdef DEBUG
#include <QDebug>
#endif

Trajectory::Trajectory(double init_x, double init_y, double velocity, double jumpAngle, double mass,
                       double airResitance, double screen_width, double screen_heigth, QGraphicsItem *parent):
    QGraphicsItem(parent), m_initX(init_x), m_initY(init_y), m_velocity(velocity),
    m_jumpAngle(jumpAngle * M_PI / 180), m_mass(mass), m_airResitance(airResitance),
    m_screenWidth(screen_width), m_screenHeight(screen_heigth), m_rModel(AirResistanceModel::NO_RESISTANCE)
{
    calcParams();

}

double Trajectory::velocity() const
{
    return m_velocity;
}

void Trajectory::setVelocity(double velocity)
{
    m_velocity = velocity;
    calcParams();
}

double Trajectory::jumpAngle() const
{
    return m_jumpAngle;
}

void Trajectory::setJumpAngle(double jumpAngle)
{
    m_jumpAngle = jumpAngle * M_PI / 180;
    calcParams();
}

double Trajectory::mass() const
{
    return m_mass;
}

void Trajectory::setMass(double mass)
{
    m_mass = mass;
    calcParams();
}

double Trajectory::airResitance() const
{
    return m_airResitance;
}

void Trajectory::setAirResitance(double airResitance)
{
    m_airResitance = airResitance;
    calcParams();
}

AirResistanceModel Trajectory::rModel() const
{
    return m_rModel;
}

void Trajectory::setRModel(const AirResistanceModel &rModel)
{
    m_rModel = rModel;
    calcParams();
}

QRectF Trajectory::boundingRect() const
{
    return QRectF(0, m_screenHeight - m_heightMax, m_distanceMax, m_heightMax);
}

void Trajectory::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    (void)option;
    (void)widget;
    QPen pen(Qt::red);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->drawPolygon(QPolygonF(m_path));
}

double Trajectory::flightTime() const
{
    return m_flightTime;
}

double Trajectory::distanceMax() const
{
    return m_distanceMax;
}

double Trajectory::heightMax() const
{
    return m_heightMax;
}

QVector<QPointF> Trajectory::path() const
{
    return m_path;
}

void Trajectory::calcParams()
{
    m_path.clear();
    double t = 0.0;
    bool isTouchGround = false;
    switch(m_rModel)
    {
        case AirResistanceModel::NO_RESISTANCE:
        /*
         * In the first case we will not take into account the air resistance force
           Trajectory parametric equations (t - time):
           X = V0 * cos(alpha) * t
           Y = V0 * sin(alpha) * t - 0.5 * g * t * t
           Flight time:
           t = 2* V0 * sin(alpha) / g;
           Maximal flight distance:
           Lmax = v0 * v0 * sin(2 * alpha) / g
           Heighest point of the trajectory(maximal flight height)
           Hmax = V0 * V0 * sin(alpha) * sin(alpha) / 2 / g;
        */
            m_flightTime = 2 * m_velocity * sin(m_jumpAngle) / GRAVITY;
            for(t = 0.0; t <= m_flightTime; t += 0.1)
            {
                const double x = m_velocity * cos(m_jumpAngle) * t;
                const double y = m_velocity * sin(m_jumpAngle) * t - 0.5 * GRAVITY * t * t;
                m_path << QPointF(x, m_screenHeight - y);
            }
            m_distanceMax = pow(m_velocity, 2) * sin(2 * m_jumpAngle) / GRAVITY;
            m_path << QPointF(m_distanceMax, m_screenHeight);
            m_heightMax = pow(m_velocity, 2) * sin(m_jumpAngle) / GRAVITY;
            break;
        case AirResistanceModel::LINEAR_RESISTANCE:
        /*
         * In the second case we will assume that air resistence force depends on ball
         * velocity linearly.
         * x = V0x * m / k *(1 - exp(-k * t / m))
         * y = m / k *[(v0y + m * g / k)(1 - exp(-k * t / m)) - g * t]
        */
            m_flightTime = 0;
            m_distanceMax = 0;
            m_heightMax = 0;
            t = 0.0;
            m_path << QPointF(0, m_screenHeight);
            do
            {
                t += 0.1;
                const double x = m_velocity * cos(m_jumpAngle) * m_mass / m_airResitance *
                        (1 - exp(-m_airResitance * t / m_mass));
                const double y = m_mass / m_airResitance *
                        ((m_velocity * sin(m_jumpAngle)  + m_mass * GRAVITY / m_airResitance) *
                        (1 - exp(-m_airResitance * t / m_mass)) - GRAVITY * t);

                if(y > 0)
                {
                    m_path << QPointF(x, m_screenHeight - y);
                    if(x > m_distanceMax) m_distanceMax = x;
                    if(y > m_heightMax) m_heightMax = y;
                }
                else
                {
                     //Linear approximation between two last trajectory points:
                     // next-to-last point has y > 0 and las - y < 0 and we will find
                     // point where ball crosses ground

                     double x2 = x, y2 = y;
                     double x1 = m_path.at(m_path.size() - 1).x();
                     double y1 = m_path.at(m_path.size() - 1).y();
                     double k = (y2 - y1) / (x2 - x1); //approximation line slope
                     double b = y1 - k * x1;
                     double x_crossGround = -b / k;
                     m_path << QPointF(x_crossGround, m_screenHeight);
                     isTouchGround = true;
                }

            } while(!isTouchGround);
            m_flightTime = t;
            break;
        default:
        case AirResistanceModel::QUADRATIC_RESISTANCE:
            /* Here we are using numerical method of integrating the differential
             *  equation system in order to calculate trajectory of the ball
             * Vx(t + dt) = vx(t) - k / m * vx(t) * dt
             * Vy(t + dt) = vy(t) - (g + k / m * vy(t)) * dt
             * x(t + dt) = x(t) + vx(t) * dt
             * y(t + dt) = y(t) + vy(t) * dt
            */
            m_flightTime = 0;
            m_distanceMax = 0;
            m_heightMax = 0;
            t = 0;
            m_path << QPointF(0, m_screenHeight);
            QPointF pos_curr(0, 0);
            QPointF v_curr(m_velocity * cos(m_jumpAngle), m_velocity * sin(m_jumpAngle));
            do
            {
                t += TIME_INCREMENT;
                pos_curr.setX(pos_curr.x() + v_curr.x() * TIME_INCREMENT);
                pos_curr.setY(pos_curr.y() + v_curr.y() * TIME_INCREMENT);
                v_curr.setX(v_curr.x() - m_airResitance / m_mass * v_curr.x() * TIME_INCREMENT);
                v_curr.setY(v_curr.y() - (GRAVITY + m_airResitance / m_mass * v_curr.y()) * TIME_INCREMENT);
                if(pos_curr.y() > 0)
                {
                    m_path << QPointF(pos_curr.x(), m_screenHeight - pos_curr.y());
                    if(pos_curr.x() > m_distanceMax) m_distanceMax = pos_curr.x();
                    if(pos_curr.y() > m_heightMax) m_heightMax = pos_curr.y();
                }
                else
                {
                    double x2 = pos_curr.x(), y2 = pos_curr.y();
                    double x1 = m_path.at(m_path.size() - 1).x();
                    double y1 = m_path.at(m_path.size() - 1).y();
                    double k = (y2 - y1) / (x2 - x1); //approximation line slope
                    double b = y1 - k * x1;
                    double x_crossGround = -b / k;
                    m_path << QPointF(x_crossGround, m_screenHeight);
                    isTouchGround = true;
                }

            } while(!isTouchGround);
            m_flightTime = t;
            break;
    }
   printPathPoints();

}
#ifdef DEBUG
void Trajectory::printPathPoints()
{
    qDebug() << "------------------------------------";
    for(auto p: m_path)
    {
        qDebug() << p;
    }
    qDebug() << "------------------------------------";
}
#endif
