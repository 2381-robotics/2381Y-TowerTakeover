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
    auto it = taskList.begin();
    while (it!= taskList.end()){
        if (it->done())
        {
            it = taskList.erase(it);
        } else {
            it->run();
            if(it->isSync) {
                break;
            } 
            it++;
        }
    }
}