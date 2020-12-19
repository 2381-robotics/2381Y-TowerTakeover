#include "main.h"
#include "autonomous/auto_timer.hpp"
#include "globals.hpp"

AutoTimer::AutoTimer(AutoTimerArgs args)
    : AutoTask{
          [&](void) -> void {
            _run_action();
            _run_increment();
          },
          [&](void) -> bool {

            return (_time >= _duration); }, args.init, args.kill, args.sync},
      _duration(args.interval), _run_action(args.task)
{
}
void AutoTimer::_run_increment()
{
  _time += DELAY_INTERVAL;
}
AutoTimer::AutoTimer(int duration, bool sync, std::function<void(void)> task, std::function<bool(void)> done, std::function<void(void)> init, std::function<void(void)> kill)
    : AutoTask{
          [&](void) -> void {
            _run_action();
            _run_increment();
          },
          [&](void) -> bool { return (_timer_done()); }, init, kill, sync},
      _duration(duration), _run_action(task), _done_check(done)
{
}
bool AutoTimer::_timer_done()
{
  return ((_time >= _duration) || _done_check());
}