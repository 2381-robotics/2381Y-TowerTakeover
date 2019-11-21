#include "main.h"
#include "auton_control.hpp"

AutonControl* AutonControl::instance() {
    static AutonControl control;
    return &control;
}
void AutonControl::run() {
     _auton_running->run();
}

void AutonControl::select_auton(std::string sequenceName){
    auto it = autonomous_registry.find(sequenceName);
    if (it != autonomous_registry.end())
    {
        _auton_running = it->second;
    }
}

void AutonControl::define_auton(std::string auton_name, AutoSequence* auton) {
    autonomous_registry[auton_name] = auton;
}