#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include <QGraphicsItem>
#include <QVector>
#include <cmath>

#define GRAVITY 9.8
#define TIME_INCREMENT 0.1

enum class AirResistanceModel{NO_RESISTANCE, LINEAR_RESISTANCE, QUADRATIC_RESISTANCE};

class Trajectory : public QGraphicsItem
{
public:
    explicit Trajectory(double init_x, double init_y, double velocity, double jumpAngle,
                        double mass, double airResitance, double width_max,
                        double height_max, QGraphicsItem *parent = nullptr);
    double velocity() const;
    double jumpAngle() const;
    double mass() const;
    double airResitance() const;
    AirResistanceModel rModel() const;
    QVector<QPointF> path() const;
    double heightMax() const;
    double distanceMax() const;
    double flightTime() const;

    void setVelocity(double velocity);
    void setJumpAngle(double jumpAngle);
    void setMass(double mass);
    void setAirResitance(double airResitance);
    void setRModel(const AirResistanceModel &rModel);
protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;
private:
    double m_initX, m_initY;
    double m_velocity, m_jumpAngle, m_mass, m_airResitance;
    double m_screenWidth, m_screenHeight;
    AirResistanceModel m_rModel;
    QVector<QPointF> m_path;
    double m_heightMax, m_distanceMax, m_flightTime;
    QPen *m_pen;
    void calcParams();
#ifdef DEBUG
    void printPathPoints(); //For debugging only
#endif
};

#endif // TRAJECTORY_H
