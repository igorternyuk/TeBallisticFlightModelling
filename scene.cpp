#include "mythread.h"
#include "scene.h"
#include "trajectory.h"
#include "ball.h"
#include <QDebug>

Scene::Scene(QObject *parent):
    QGraphicsScene(m_const.SCENE_LEFT, m_const.SCENE_TOP, m_const.SCENE_WIDTH, m_const.SCENE_HEIGHT, parent)
{
    m_thread = new MyThread(m_const.DELAY, this);
    m_path = new Trajectory(m_const.SCENE_LEFT, m_const.SCENE_TOP, m_const.BALL_INITIAL_VELOCITY,
                            m_const.BALL_INITIAL_JUMP_ANGLE, m_const.BALL_INITIAL_MASS,
                            m_const.INITIAL_AIR_RESISTANCE, m_const.SCENE_WIDTH,
                            m_const.SCENE_HEIGHT);
    m_ball = new Ball(m_const.SCENE_LEFT, m_const.SCENE_HEIGHT, m_const.BALL_RADIUS);
    this->addRect(m_const.SCENE_LEFT, m_const.SCENE_TOP, m_const.SCENE_WIDTH, m_const.SCENE_HEIGHT);
    this->addItem(m_path);
    this->addItem(m_ball);

    connect(m_thread, &MyThread::timeOut, [&](){
        if(m_ball->isMoving())
        {
            if(m_index < 0 || m_index > m_path->path().size() - 1)
            {
                m_index = 0;
            }
            const double nextX = m_path->path().at(m_index).x();
            const double nextY = m_path->path().at(m_index).y();
            m_ball->setX(nextX);
            m_ball->setY(nextY);
            ++m_index;
            update();
        }
    });

    m_thread->start();
}

Scene::myConsts Scene::m_const;

Scene::~Scene()
{
    m_thread->quit();
    if(!m_thread->wait(m_const.WAIT_TIME)) //Wait until it actually has terminated (max. 3 sec)
    {
        m_thread->terminate(); //Thread didn't exit in time, probably deadlocked, terminate it!
        m_thread->wait(); //We have to wait again here!
    }
}

void Scene::changeBallVelocity(double val)
{
    m_path->setVelocity(val);
    emitSignals();
    update();
}

void Scene::changeBallMass(double val)
{
    m_path->setMass(val);
    emitSignals();
    update();
}

void Scene::changeJumpAngle(double val)
{
    m_path->setJumpAngle(val);
    emitSignals();
    update();
}

void Scene::chageAirResistance(double val)
{
    m_path->setAirResitance(val);
    emitSignals();
    update();
}

void Scene::setSimpleModel()
{
    m_path->setRModel(AirResistanceModel::NO_RESISTANCE);
    emitSignals();
    update();
}

void Scene::setLinearResistanceModel()
{
    m_path->setRModel(AirResistanceModel::LINEAR_RESISTANCE);
    emitSignals();
    update();
}

void Scene::setQuadraticResistanceModel()
{
    m_path->setRModel(AirResistanceModel::QUADRATIC_RESISTANCE);
    emitSignals();
    update();
}

void Scene::toggleAnimation()
{
    if(m_ball->isMoving())
    {
        m_ball->setIsMoving(false);
        emit animationStopped();
    }
    else
    {
        m_ball->setIsMoving(true);
        emit animationStarted();
    }
}

void Scene::emitSignals()
{
    emit flightTimeChanged(QString::number(m_path->flightTime(), 'g', 6));
    emit flightDistanceChanged(QString::number(m_path->distanceMax(), 'g', 6));
    emit flightHeightMaxChanged(QString::number(m_path->heightMax(), 'g', 6));
}
