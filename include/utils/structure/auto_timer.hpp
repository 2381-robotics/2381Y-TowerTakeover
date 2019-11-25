#ifndef AUTO_TIMER_HPP
#define AUTO_TIMER_HPP

#include <functional>
#include <future>
#include "auto_task.hpp"
class AutoTimer : public AutoTask
{
public:
    int _time = 0;
    int _duration = 0;
    AutoTimer(int interval, bool sync = true, std::function<void(void)> task = [](void) -> void {}, std::function<void(void)> init = [](void) -> void {}, std::function<void(void)> kill = [](void) -> void {});
protected:
};

#endif