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

    static AutoTask AsyncTask(std::function<void(void)> task, std::function<bool(void)> done, std::function<void(void)> init = [](void) -> void {}, std::function<void(void)> kill = [](void) -> void {});
    static AutoTask SyncTask(std::function<void(void)> task, std::function<bool(void)> done, std::function<void(void)> init = [](void) -> void {}, std::function<void(void)> kill = [](void) -> void {});
    static AutoTask AutoDelay(int interval, bool sync = true, std::function<void(void)> task = [](void) -> void {}, std::function<void(void)> init = [](void) -> void {}, std::function<void(void)> kill = [](void) -> void {});

    bool isSync;
    bool _initialized = false;
    int _count = 0;
    AutoTask(std::function<void(void)> task, std::function<bool(void)> done, std::function<void(void)> init = [](void)-> void{}, std::function<void(void)> kill = [](void)->void{});
protected:
};

#endif