#include "stopwatch.h"

// public:

Stopwatch::Stopwatch(unsigned int resolution_in_milliseconds, QObject* parent)
    : resolution(resolution_in_milliseconds),
      QObject(parent)
{
    timer = new QTimer(this);
    timer->setSingleShot(false);
    
    current_value = 0;
    last_round_value = 0;
    round_number = 0;
    
    QObject::connect(timer, &QTimer::timeout, this, &Stopwatch::onTimerTimeout);
}


void Stopwatch::start()
{
    timer->start(resolution);
}


void Stopwatch::stop()
{
    timer->stop();
}


QString Stopwatch::reset()
{
    current_value = 0;
    last_round_value = 0;
    round_number = 0;
    
    return toTimeStrFormat(current_value);
}


QString Stopwatch::round()
{
    const unsigned int prev_round_value = last_round_value;
    last_round_value = current_value;
    
    return ("Круг №" + QString::number(++round_number) + ", время: " + toTimeStrFormat(last_round_value - prev_round_value) + " сек");
}


void Stopwatch::primaryHandshake()
{
    emit ding(toTimeStrFormat(0));
}


// public slots:

void Stopwatch::onTimerTimeout()
{
    current_value += resolution;
    emit ding(toTimeStrFormat(current_value));
}


// private:

QString Stopwatch::toTimeStrFormat(const unsigned int milliseconds)
{
    const unsigned int minutes = milliseconds / 60000;
    const unsigned int seconds = (milliseconds % 60000) / 1000;
    const unsigned int portions_of_second = (milliseconds % 1000) / resolution;
    
    return (minutes < 10 ? "0" + QString::number(minutes) : QString::number(minutes)) + ":" + 
            (seconds < 10 ? "0" + QString::number(seconds) : QString::number(seconds)) + "." + 
            QString::number(portions_of_second);
}
