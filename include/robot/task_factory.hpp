#ifndef TASK_FACTORY_HPP_
#define TASK_FACTORY_HPP_
#include <functional>
#include "robot/module.hpp"
#include "api.h"
struct Task_Factory{
    public: 
    static pros::task_fn_t Create_Task(Module* module);
};
#endif