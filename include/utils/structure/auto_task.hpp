#ifndef AUTO_TASK_HPP
#define AUTO_TASK_HPP

#include <functional>
#include <future>

class AutoTask {
    public: 
    std::function<bool(void)> done;
    std::function<void(void)> run;
    std::function<void(void)> init;
    std::function<void(void)> kill;
    static AutoTask AsyncTask(std::function<void(void)> task, std::function<bool(void)> done, std::function<void(void)> init);
    static AutoTask SyncTask(std::function<void(void)> task, std::function<bool(void)> done, std::function<void(void)> init);
    bool isSync;
    bool initialized = false;
private:
    AutoTask(std::function<void(void)> task, std::function<bool(void)> done, std::function<void(void)> init);
};

#endif