#include "main.h"
#include "autonomous/auto_task.hpp"
#include "autonomous/auto_timer.hpp"

AutoTask AutoTask::AsyncTask(std::function<void(void)> task, std::function<bool(void)> done, std::function<void(void)> init, std::function<void(void)> kill)
{
    AutoTask *asyncTask = new AutoTask(task, done, init, kill, false);
    return *asyncTask;
}
AutoTask AutoTask::SyncTask(std::function<void(void)> task, std::function<bool(void)> done, std::function<void(void)> init, std::function<void(void)> kill)
{
    AutoTask *syncTask = new AutoTask(task, done, init, kill, true);
    return *syncTask;
}
AutoTask AutoTask::AutoDelay(int time, bool sync, std::function<void(void)> task, std::function<void(void)> init, std::function<void(void)> kill)
{
    return *new AutoTimer({interval : time, sync : sync, task : task, init : init, kill : kill});
}




AutoTask::AutoTask(std::function<void(void)> task, std::function<bool(void)> done, std::function<void(void)> init, std::function<void(void)> kill, bool sync)
    : isSync(sync)
{
    this->doneList.push_back(done);
    this->runList.push_back(task);
    this->initList.push_back(init);
    this->killList.push_back(kill);
}

AutoTask &AutoTask::AddRun(std::function<void(void)> task)
{
    runList.push_back(task);
    return *this;
}

AutoTask &AutoTask::AddInit(std::function<void(void)> task)
{
    initList.push_back(task);
    return *this;
}


AutoTask &AutoTask::AddKill(std::function<void(void)> task)
{
    killList.push_back(task);
    return *this;
}

AutoTask &AutoTask::AddDone(std::function<bool(void)> done)
{
    doneList.push_back(done);
    return *this;
}

void AutoTask::run(void)
{
    for (const auto &value : this->runList)
    {
        value();
    }
}

void AutoTask::kill(void)
{
    for (const auto &value : this->killList)
    {
        value();
    }
}
void AutoTask::initialize(void)
{
    for (const auto &value : this->initList)
    {
        value();
    }
}
bool AutoTask::done(void)
{
    auto isDone = false;
    for (const auto &value : this->doneList)
    {
        isDone = isDone || value();
    }
    return isDone;
}
AutoTask AutoTask::TimeLimit(int time)
{

    AutoTask *timedTask = new AutoTimer(time);
    for (const auto &value : this->runList)
    {
        timedTask->AddRun(value);
    };
    for (const auto &value : this->killList)
    {
        timedTask->AddKill(value);
    }
    for (const auto &value : this->initList)
    {
        timedTask->AddInit(value);
    }
    for (const auto &value : this->doneList)
    {
        timedTask->AddDone(value);
    }
    return *timedTask;
}
