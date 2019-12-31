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
    int j = 0;

    while (it!= taskList.end()){
        if (!it->_initialized)
        {
            it->initialize();
            it->_initialized = true;
        }
        if (it->done())
        {
            // pros::lcd::set_text(5, "Taskdone");
            it->kill();
            
            it = taskList.erase(it);
            if (it->isSync)
            {
                break;
            }
        } else {
            
            it->run();
            // break;
            if(it->isSync) {
                pros::lcd::set_text(6, "j " + to_string(j));
                break;
                return;
            }
        }
        it++;
        j++;
    }
}
