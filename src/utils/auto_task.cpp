#include "main.h"
#include "utils/structure/auto_task.hpp"

AutoTask AutoTask::AsyncTask(std::function<void(void)> task, std::function<bool(void)> done){
    AutoTask asyncTask = AutoTask(task, done);
    asyncTask.isSync = false;
    return asyncTask;
}
AutoTask AutoTask::SyncTask(std::function<void(void)> task, std::function<bool(void)> done)
{
    AutoTask asyncTask = AutoTask(task, done);
    asyncTask.isSync = true;
    return asyncTask;
}
AutoTask::AutoTask(std::function<void(void)> task, std::function<bool(void)> done){
    this->done = done;
    this->run = task;
}
