#ifndef SIGNALS_H
#define SIGNALS_H

#include <QObject>

class Signals : public QObject
{
    Q_OBJECT
    
public:
    explicit Signals(QObject* parent = nullptr) : QObject(parent) {}
    
    void send_ding(const unsigned int value)
    {
        emit ding(value);
    }
    
signals:
    void ding(const unsigned int value);
    
};

#endif // SIGNALS_H
