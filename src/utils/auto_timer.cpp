#include "main.h"
#include "utils/structure/auto_timer.hpp"
#include "globals.hpp"

AutoTimer::AutoTimer(int duration, bool sync, std::function<void(void)> task, std::function<void(void)> init, std::function<void(void)> kill)
    : AutoTask{
          [&](void) -> void {
              task();
              _time += DELAY_INTERVAL;
          },
          [&](void) -> bool { return _time < _duration; }, init, kill}
{
    _duration = duration;
    this->isSync = sync;
}