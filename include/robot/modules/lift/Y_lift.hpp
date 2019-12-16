#ifndef Y_LIFT_HPP
#define Y_LIFT_HPP

#include "robot/modules/lift/lift.hpp"
#include "robot/control/motor_controller.hpp"
#include "api.h"
#include "okapi/api.hpp"
 
class Y_Lift : public Lift{
public:
    Y_Lift();
    void Set_Target(double target_height) override;
    void Move_Lift() override;
    double Get_Height() override;
    double Get_Target() override;
    void create() override;

    std::string name = "Y_Lift";

    void Increment_Height(int increment) override;
    void Smooth_Lift(int increment) override;
private:
    pros::Motor* _left_lift_motor;
    pros::Motor* _right_lift_motor;
    Motor_Controller* _left_lift_motor_controller;
    Motor_Controller *_right_lift_motor_controller;
    Pid* _master_lift_pid;
    Pid* _lift_pid;

    double _lift_coefficient, _cube_height, _lift_difference;
};


#endif