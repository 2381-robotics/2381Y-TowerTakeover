#include "main.h"
#include "autonomous/auton_control.hpp"

AutonControl* AutonControl::instance() {
    static AutonControl control;
    return &control;
}
void AutonControl::run() {
     _auton_running->run();
}


void AutonControl::select_auton(Autonomous sequence){
    auto it = autonomous_registry.find(sequence);
    if (it != autonomous_registry.end())
    {
        _auton_running = it->second;
    }
}   

void AutonControl::define_auton(Autonomous sequence, AutoSequence* auton) {
    autonomous_registry[sequence] = auton;
}