#include "main.h"
#include "autonomous/auto_timer.hpp"
#include "globals.hpp"

AutoTimer::AutoTimer(int duration, bool sync, std::function<void(void)> task, std::function<void(void)> init, std::function<void(void)> kill)
    : AutoTask{
          [&](void) -> void {
            _run_action();
            _run_increment();
          },
          [&](void) -> bool { return (_time >= _duration); }, init, kill, sync},
      _duration(duration), _run_action(task)
{}
void AutoTimer::_run_increment() {
  _time += DELAY_INTERVAL;
}
void AutoTimer::_run() {

}