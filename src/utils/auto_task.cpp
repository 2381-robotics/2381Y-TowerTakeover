#include "main.h"
#include "utils/structure/auto_task.hpp"

AutoTask AutoTask::AsyncTask(std::function<void(void)> task, std::function<bool(void)> done, std::function<void(void)> init){
    AutoTask asyncTask = AutoTask(task, done, init);
    asyncTask.isSync = false;
    return asyncTask;
}
AutoTask AutoTask::SyncTask(std::function<void(void)> task, std::function<bool(void)> done, std::function<void(void)> init)
{
    AutoTask asyncTask = AutoTask(task, done, init);
    asyncTask.isSync = true;
    return asyncTask;
}
AutoTask::AutoTask(std::function<void(void)> task, std::function<bool(void)> done, std::function<void(void)> init){
    this->done = done;
    this->run = task;
    this->init = init;
}
