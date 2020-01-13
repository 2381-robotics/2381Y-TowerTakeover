#include "main.h"
#include "autonomous/auto_task.hpp"
#include "autonomous/auto_sequence.hpp"
#include <vector>
using namespace std;

AutoSequence* AutoSequence::FromTasks(vector<AutoTask> tasks) {
    AutoSequence* instance = new AutoSequence();
    instance->add_tasks(tasks);
    return instance;
}

void AutoSequence::add_tasks(vector<AutoTask> tasks){
    taskList.reserve(taskList.size() + tasks.size());
    taskList.insert(taskList.end(), tasks.begin(), tasks.end());
}
AutoSequence::AutoSequence() {}



void AutoSequence::run(){
    auto it = taskList.begin();

    while (it!= taskList.end()){
        if (!it->_initialized)
        {
            it->initialize();
            it->_initialized = true;
        }

        it->run();
        if (it->done())
        {
            // pros::lcd::set_text(5, "Taskdone");
            it->kill();
            it = taskList.erase(it);   
        }
        if (it->isSync)
        {
            break;
        }
        it++;
    }
}
