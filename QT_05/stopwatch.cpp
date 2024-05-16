#include "stopwatch.h"

#include <chrono>
#include <cstdlib>

// (main thread)
Stopwatch::Stopwatch(const unsigned int resolution_in_milliseconds, Signals* associated_recieving_obj)
    : resolution(resolution_in_milliseconds)
    , associated_reciever(associated_recieving_obj)
{
    current_value = 0;
    current_portions_amount = 0;
    working_status = false;
    stopped = true;
    destruct = false;
    side_thread = std::make_unique<std::thread>(stand_by, this);
}

// (main thread)
Stopwatch::~Stopwatch()
{
    working_status = false;
    destruct = true;
    start_cond.notify_all();
    side_thread->join();
}

// (main thread)
void Stopwatch::start()
{
    {
        std::lock_guard<std::mutex> lk(mut);
        stopped = false;
        working_status = true;
    }
    start_cond.notify_one();
}

// (main thread)
void Stopwatch::stop()
{
    working_status = false;
    
    std::unique_lock<std::mutex> lk(mut);
    start_cond.wait(lk, [this]{return (this->stopped || this->destruct);});
    lk.unlock();
}

// (main thread)
void Stopwatch::reset()
{
    if (working_status) { stop(); }
    current_value = 0;
    current_portions_amount = 0;
    notify_ui();
}

// (main thread)
unsigned int Stopwatch::get_current()
{
    return current_value;
}

// (side thread)
void Stopwatch::stand_by(Stopwatch* sw)
{
    while (!sw->destruct)
    {
        std::unique_lock<std::mutex> lk(sw->mut);
        sw->start_cond.wait(lk, [&sw] { return sw->working_status || sw->destruct; });
        
        if (sw->working_status)
        {
            sw->work();
            sw->stopped = true;
        }
        lk.unlock();
        sw->start_cond.notify_one();
    }
}

// (side thread)
void Stopwatch::work()
{
    const auto start = std::chrono::steady_clock::now();
    const unsigned int start_milliseconds = current_value;
    const unsigned int start_portions = current_portions_amount;
    unsigned int milliseconds_passed = 0;
    
    while (working_status)
    {
        auto now = std::chrono::steady_clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
        milliseconds_passed = diff.count();
        
        current_value = start_milliseconds + milliseconds_passed;
        
        if (current_value / resolution != current_portions_amount)
        {
            current_portions_amount = current_value / resolution;
            notify_ui();
        }
    }
}

// (side thread)
void Stopwatch::notify_ui()
{
    associated_reciever->send_ding(current_value);
}
