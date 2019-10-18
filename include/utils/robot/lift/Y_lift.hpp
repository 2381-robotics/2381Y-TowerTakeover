#ifndef Y_LIFT_HPP
#define Y_LIFT_HPP

#include "utils/robot/lift/lift.hpp"
#include "utils/motor_controller.hpp"
#include "api.h"
#include "okapi/api.hpp"

class Y_Lift : public Lift{
public:
    Y_Lift();
    void Set_Height(double target_height) override;
    double Get_Height() override;
    double GetTarget() override;
    void create() override;
private:
    pros::Motor* _left_lift_motor;
    pros::Motor* _right_lift_motor;
    Motor_Controller* _left_lift_motor_controller;
    Motor_Controller *_right_lift_motor_controller;
    Pid* _master_lift_pid;
    Pid* _lift_pid;

    double _lift_coefficient;
    double _lift_power;
};


#endif