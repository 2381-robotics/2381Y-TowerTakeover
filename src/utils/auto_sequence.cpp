#include "main.h"
#include "utils/structure/auto_task.hpp"
#include "utils/structure/auto_sequence.hpp"
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

void AutoSequence::next(){
    pros::lcd::set_text(7, "Auton Control");
    int numberRunning = 0;
    auto it = taskList.begin();
    while (it!= taskList.end()){
        numberRunning++;
        if (!it->_initialized)
        {
            it->initialize();
            it->_initialized = true;
        }
        if (it->done())
        {
            it->kill();
            it = taskList.erase(it);
        } else {
            it->run();
            // break;
            if(it->isSync) {
                break;
                // return;
            }
        }
        it++;
    }
    pros::lcd::set_text(4, to_string(numberRunning));
}