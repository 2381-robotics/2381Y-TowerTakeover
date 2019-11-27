#include "main.h"
#include "utils/structure/auto_task.hpp"
#include "utils/structure/auto_timer.hpp"

AutoTask AutoTask::AsyncTask(std::function<void(void)> task, std::function<bool(void)> done, std::function<void(void)> init, std::function<void(void)> kill)
{
    AutoTask asyncTask = AutoTask(task, done, init, kill);
    asyncTask.isSync = false;
    return asyncTask;
}
AutoTask AutoTask::SyncTask(std::function<void(void)> task, std::function<bool(void)> done, std::function<void(void)> init, std::function<void(void)> kill)
{
    AutoTask asyncTask = AutoTask(task, done, init, kill);
    asyncTask.isSync = true;
    return asyncTask;
}
AutoTask AutoTask::AutoDelay(int time, bool sync, std::function<void(void)> task, std::function<void(void)> init, std::function<void(void)> kill){
    return AutoTimer(time, sync, task, init, kill);
}

AutoTask::AutoTask(std::function<void(void)> task, std::function<bool(void)> done, std::function<void(void)> init, std::function<void(void)> kill)
{
    this->done = done;
    this->run = task;
    this->initialize = init;
    this->kill = kill;
}
