#include "module.hpp"
#include "globals.hpp"


void Module::Run() {
    if(STOP){
        Stop();
        return;
    }
    this->Move_Motor();
    // pros::lcd::set_text(0, to_string(pros::millis()));
}