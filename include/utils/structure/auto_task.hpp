#ifndef AUTO_TASK_HPP
#define AUTO_TASK_HPP

#include <functional>
#include <future>

class AutoTask {
    public: 
    std::function<bool(void)> done;
    std::function<void(void)> run;
    static AutoTask AsyncTask(std::function<void(void)> task, std::function<bool(void)> done);
    static AutoTask SyncTask(std::function<void(void)> task, std::function<bool(void)> done);
    bool isSync;
private:
    AutoTask(std::function<void(void)> task, std::function<bool(void)> done);
};

#endif