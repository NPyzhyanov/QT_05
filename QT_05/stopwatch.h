#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QObject>
#include <QTimer>

class Stopwatch : public QObject
{
    Q_OBJECT
    
public:
    explicit Stopwatch( unsigned int resolution_in_milliseconds, QObject* parent = nullptr);
    
    void start();
    void stop();
    QString reset();
    QString round();
    
    void primaryHandshake();
    
public slots:
    void onTimerTimeout();
    
signals:
    void ding(QString new_value);
    
private:
    QString toTimeStrFormat(const unsigned int milliseconds);
    
    QTimer* timer;
    const unsigned int resolution; // ms
    
    unsigned int current_value; // ms
    unsigned int last_round_value; // ms
    unsigned int round_number;
};

#endif // STOPWATCH_H
