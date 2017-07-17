#include "mythread.h"

MyThread::MyThread(int delay, QObject *parent):
    QThread(parent), m_delay(delay)
{}

void MyThread::run()
{
    while(true)
    {
        msleep(m_delay);
        emit timeOut();
    }
}
