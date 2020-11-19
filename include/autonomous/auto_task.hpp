#ifndef AUTO_TASK_HPP
#define AUTO_TASK_HPP

#include <functional>
#include <vector>




class AutoTask {
    public: 
    std::function<bool(void)> done;

    void run(void);

    std::function<void(void)> initialize;
    std::function<void(void)> kill;

    std::vector<std::function<void(void)>> runList = {};
    std::vector<std::function<void(void)>> doneList = {};
    std::vector<std::function<void(void)>> initList = {};

    static AutoTask AsyncTask(std::function<void(void)> task, std::function<bool(void)> done, std::function<void(void)> init = [](void) -> void {}, std::function<void(void)> kill = [](void) -> void {});
    static AutoTask SyncTask(std::function<void(void)> task, std::function<bool(void)> done, std::function<void(void)> init = [](void) -> void {}, std::function<void(void)> kill = [](void) -> void {});
    static AutoTask AutoDelay(int interval, bool sync = true, std::function<void(void)> task = [](void) -> void {}, std::function<void(void)> init = [&](void) -> void {}, std::function<void(void)> kill = [](void) -> void {});
    // static AutoTask DriveTask()

    AutoTask TimeLimit(int time);
    
    bool isSync;
    bool _initialized = false;
    AutoTask(std::function<void(void)> task, std::function<bool(void)> done, std::function<void(void)> init = [](void)-> void{}, std::function<void(void)> kill = [](void)->void{}, bool sync = true);

    AutoTask& AddRun(std::function<void(void)> task);

protected:
};

#endif