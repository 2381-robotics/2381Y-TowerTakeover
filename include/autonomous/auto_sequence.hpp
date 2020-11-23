#ifndef AUTO_SEQUENCE_HPP
#define AUTO_SEQUENCE_HPP

#include <functional>
#include <future>
#include <vector>
#include "auto_task.hpp"

class AutoSequence : public AutoTask{
    public: 

    int tasksRemaining = 0;
    void run_sequence();
    void Reset();
    bool isSequenceFinished = false;
    
    std::vector<AutoTask> taskList;
    std::vector<AutoTask> resetTaskList;
    
    void add_tasks(std::vector<AutoTask> tasks);

    static AutoSequence* FromTasks(std::vector<AutoTask> tasks);
private:
    AutoSequence();
};

#endif