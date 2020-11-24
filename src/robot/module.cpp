#include "module.hpp"
#include "globals.hpp"
#include "structure.hpp"


void Module::Run() {
    if(STOP){
        Stop();
        return;
    }
    this->Move_Motor();
}

void Structure::Init()
{
    Create();
    isInitialized = true;
}