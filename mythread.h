#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>

class MyThread : public QThread
{
    Q_OBJECT

public:
    explicit MyThread(int delay, QObject *parent = nullptr);
signals:
    void timeOut();
protected:
    void run() override;
private:
    int m_delay;
};

#endif // MYTHREAD_H
