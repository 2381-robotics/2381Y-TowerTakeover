#ifndef INTAKE_HPP
#define INTAKE_HPP

#include "utils/robot/drive/drive.hpp"
#include "utils/motor_controller.hpp"
#include <array>

class Intake
{
public:
    Intake();
    void Set_Intake(double upL, double downL);
    double Get_Speed();
    void create();


private:
    Motor_Controller *rightIntakeController;
    Motor_Controller *leftIntakeController;

    Pid *_master_intake_pid;
    double _intake_setpoint;
    double _outtake_setpoint;

    double _left_intake_value;
    double _right_intake_value;
    double _intake_value_average;
    double _master_intake_setpoint;
    double _master_intake_error_average;


};
#endif
