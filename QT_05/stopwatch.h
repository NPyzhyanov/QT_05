#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <thread>
#include <atomic>
#include <condition_variable>
#include <mutex>

#include "signals.h"

class Stopwatch
{
public:
    Stopwatch(const unsigned int resolution_in_milliseconds, Signals* associated_recieving_obj);
    ~Stopwatch();
    
    void start();
    void stop();
    void reset();
    
    unsigned int get_current();
    
private:
    static void stand_by(Stopwatch* sw);
    void work();
    void notify_ui();
    
    const unsigned int resolution; // ms
    
    unsigned int current_value; // ms
    unsigned int current_portions_amount; // безразм.
    
    std::unique_ptr<std::thread> side_thread;
    std::atomic<bool> working_status;
    std::atomic<bool> stopped;
    std::atomic<bool> destruct;
    std::condition_variable start_cond;
    std::mutex mut;
    
    Signals* associated_reciever;
};

#endif // STOPWATCH_H
