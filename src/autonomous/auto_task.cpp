#include "main.h"
#include "autonomous/auto_task.hpp"
#include "autonomous/auto_timer.hpp"

AutoTask AutoTask::AsyncTask(std::function<void(void)> task, std::function<bool(void)> done, std::function<void(void)> init, std::function<void(void)> kill)
{
    AutoTask* asyncTask = new AutoTask(task, done, init, kill, false);
    return *asyncTask;
}
AutoTask AutoTask::SyncTask(std::function<void(void)> task, std::function<bool(void)> done, std::function<void(void)> init, std::function<void(void)> kill)
{
    AutoTask* syncTask = new AutoTask(task, done, init, kill, true);
    return *syncTask;
}
AutoTask AutoTask::AutoDelay(int time, bool sync, std::function<void(void)> task, std::function<void(void)> init, std::function<void(void)> kill){
    return *new AutoTimer(time, sync, task, init, kill);
}

AutoTask::AutoTask(std::function<void(void)> task, std::function<bool(void)> done, std::function<void(void)> init, std::function<void(void)> kill, bool sync)
: isSync(sync)
{
    this->done = done;
    this->run = task;
    this->initialize = init;
    this->kill = kill;
}
