#ifndef MYTHREAD_H
#define MYTHREAD_H

#include "common.h"
#include "force.h"
#include "angle.h"
#include "temperature.h"
#include "motor.h"
#include "wind.h"

#include <QTime>
#include <QElapsedTimer>

#include "measurementsmodel.h"
#include "variable.h"

class MeasureThread : public QObject
{
    Q_OBJECT
public:
    explicit MeasureThread(MeasurementsModel *measurement, QObject *parent = 0);
    explicit MeasureThread(ZeroModel *measurement, QObject *parent = 0);
    ~MeasureThread();
    void isReady(void);

signals:
  void MeasureDone(QHash<QString,double>);

public slots:
    void produce(void);
    void stop();
    void manual_control(QHash<QString,double>);

private:
    void init(QList<VariableModel*> list);
    void set_m(void);
    void set_initial(void);
    void read_m(void);

    QElapsedTimer timer;

    QHash<QString,double> m_hash;
    QHash<QString,double> start_hash;

    int average_number;
    double settling_time;
    double end;
    double step;
    double current;

    QString control;

    bool isZero;
    QString name;
    int n;

    bool m_stop;
    QThread* m_parent_thread;
    bool virtual_measures;

    // Variables
    QList<VariableHardware*> variables;
};

#endif // MYTHREAD_H
