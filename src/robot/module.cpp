#include "module.hpp"
#include "globals.hpp"


void Module::Run() {
    if(STOP){
        Stop();
        return;
    }
    this->Move_Motor();
}
