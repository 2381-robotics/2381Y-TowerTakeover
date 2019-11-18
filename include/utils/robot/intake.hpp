#ifndef MECH_DRIVE_HPP
#define MECH_DRIVE_HPP

#include "utils/robot/drive/drive.hpp"
#include "utils/motor_controller.hpp"
#include <array>

class intake
{
public:
    intake();
    void Set_Drive(double upL, double downL);
    double Get_Speed();


private:
    Motor_Controller *rightIntakeController;
    Motor_Controller *leftIntakeController;

    Pid *_master_intake_pid;
    double _intake_setpoint;
    double _outtake_setpoint;

    double _left_intake_motor_value;
    double _right_intake_motor_value;
    double _intake_value_average;
    double _master_intake_setpoint;
    double _master_intake_error_average;
};
#endif
