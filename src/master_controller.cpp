#include "master_controller.hpp"
#include "main.h"
#include "ports.h"
#include "globals.hpp"
#include <map>
using namespace pros;
using namespace std;

MasterController *MasterController::instance()
{
    static MasterController factory;
    return &factory;
}

void MasterController::run() {
    lcd::set_text(7, "CURRENT STATE : " +  stateList[State(_active_mode)]);
    if(Competition_Env){
        driver();
        return;
    }
    if(master.get_digital(DIGITAL_L1)&&master.get_digital(DIGITAL_L2)&&master.get_digital(DIGITAL_R1)&&master.get_digital(DIGITAL_R2)&&master.get_digital(DIGITAL_RIGHT)&&master.get_digital(DIGITAL_Y)) {
        set_state(auton_mode);
    }

    switch(_active_mode) {
        case driver_mode:
            driver();
            break;
        case auton_mode:
            autonomous();
            break; 
        case selector_mode:
            selector();
            break;
        case debug_mode:
            debug();
            break;
        case auton_edit_mode:
            auton_editor();
            break;
        default:
            driver();
    }
}

std::map<MasterController::State, std::string> MasterController::stateList = {
    {driver_mode, "driver_mode"},
    {auton_mode, "auton_mode"},
    {selector_mode, "selector_mode"},
    {debug_mode, "debug_mode"},
    {auton_edit_mode, "auton edit mode"}
};

void MasterController::selector()
{
    for (auto it = stateList.begin(); it != stateList.end(); it++) {
        string select_prefix = it->first == _selected_mode ? "      " : "";
        lcd::set_text(distance(stateList.begin(), it), select_prefix + it->second);
    }
    _selected_mode -= master.get_digital_new_press(DIGITAL_UP) - master.get_digital_new_press(DIGITAL_DOWN);
    _selected_mode = _selected_mode % stateList.size();

    if(master.get_digital_new_press(DIGITAL_A)){ 
        set_state(State(_selected_mode));
    }
    
}

void MasterController::autonomous() {
    // lcd::set_text(1, "Increment Value: " + to_string(_autonomous_increment));

    if (master.get_digital(DIGITAL_X)){
        auton_control->run();
        STOP = false;
    } else {
        STOP = true;
    }
    if(master.get_digital(DIGITAL_B)){
        robot->stop();
    }
    if(master.get_digital_new_press(DIGITAL_Y)){
        set_state(auton_edit_mode);
    }
    lcd::set_text(1, "Current Increment Value: " + to_string(autonomous_increment));
    lcd::set_text(2, "Set To " + to_string(_local_increment));

    _local_increment += master.get_digital(DIGITAL_UP) - master.get_digital(DIGITAL_DOWN) + (master.get_digital_new_press(DIGITAL_LEFT) - master.get_digital_new_press(DIGITAL_RIGHT)) * 30;
    if (master.get_digital_new_press(DIGITAL_A))
    {
        autonomous_increment = _local_increment;
        resetAuton1();
        auton_control->define_auton("auton1", auton1);
        auton_control->select_auton("auton1");
        set_state(auton_mode);
    }

    if ( master.get_digital_new_press(DIGITAL_L2)){
        autonomous_increment = _local_increment;
        resetAuton1(1);
        auton_control->define_auton("auton2", auton1);
        auton_control->select_auton("auton2");
        set_state(auton_mode);
    }

};

void MasterController::auton_editor() {
    lcd::set_text(1, "Current Increment Value: " + to_string(_autonomous_increment));
    lcd::set_text(2, "Set To " + to_string(_local_increment));
    lcd::set_text(3, "A : SAVE");
    lcd::set_text(4, "X : RESET");

    _local_increment += master.get_digital(DIGITAL_UP) - master.get_digital(DIGITAL_DOWN) + (master.get_digital_new_press(DIGITAL_LEFT) - master.get_digital_new_press(DIGITAL_RIGHT))* 30;
    if(master.get_digital_new_press(DIGITAL_A)){
        _autonomous_increment = _local_increment;
        resetAuton1();
        auton_control->define_auton("auton1", auton1);
        auton_control->select_auton("auton1");
        set_state(auton_mode);
    } else if (master.get_digital_new_press(DIGITAL_X)){
        _local_increment = _autonomous_increment;
    }
}

void MasterController::debug() {
    for(auto it = robot->module_list.begin(); it != robot->module_list.end(); it++) {
        string select_prefix = it->first ==  (_selected_module) ? "      " : "";
        lcd::set_text(distance(robot->module_list.begin(), it), select_prefix + it->second->name);
    }
    _selected_module -= master.get_digital_new_press(DIGITAL_UP) - master.get_digital_new_press(DIGITAL_DOWN);
    _selected_module = _selected_mode % stateList.size();

    if (master.get_digital_new_press(DIGITAL_A))
    {
        set_state(State(_selected_mode));
    }
}
void MasterController::set_state(State state){
    for(int i = 0; i <8 ; i++){
        lcd::clear_line(i);
    }
    _active_mode = state;
}
