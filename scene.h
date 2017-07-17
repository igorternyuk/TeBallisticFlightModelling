#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include "trajectory.h"

/*#define SCENE_LEFT 0.0
#define SCENE_TOP 0.0
#define SCENE_WIDTH 940.0
#define SCENE_HEIGHT 600.0
#define BALL_RADIUS 5
#define DELAY 17
#define WAIT_TIME 3000*/

class Ball;
class MyThread;

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);
    ~Scene();
    void emitSignals();
signals:
    void flightTimeChanged(QString val);
    void flightDistanceChanged(QString val);
    void flightHeightMaxChanged(QString val);
    void animationStarted();
    void animationStopped();
public slots:
    void changeBallVelocity(double val);
    void changeBallMass(double val);
    void changeJumpAngle(double val);
    void chageAirResistance(double val);
    void setSimpleModel();
    void setLinearResistanceModel();
    void setQuadraticResistanceModel();
    void toggleAnimation();
private:
    static struct myConsts
    {
        double SCENE_LEFT = 0.0;
        double SCENE_TOP  = 0.0;
        double SCENE_WIDTH = 940.0;
        double SCENE_HEIGHT = 600.0;
        double BALL_RADIUS = 7;
        double BALL_INITIAL_VELOCITY = 50.0;
        double BALL_INITIAL_JUMP_ANGLE = 45.0;
        double BALL_INITIAL_MASS = 0.5;
        double INITIAL_AIR_RESISTANCE = 0.003;
        int DELAY = 30;
        int WAIT_TIME = 3000;
    } m_const;
    MyThread *m_thread;
    Trajectory *m_path;
    Ball *m_ball;
    int m_index = 0;

};

#endif // SCENE_H
