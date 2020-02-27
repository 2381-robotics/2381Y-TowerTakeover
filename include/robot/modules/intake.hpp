#ifndef INTAKE_HPP
#define INTAKE_HPP

#include "robot/modules/drive/drive.hpp"
#include "robot/control/motor_controller.hpp"
#include <array>
#include "robot/module.hpp"

#include <array>

class Intake : public Module
{
public:
    Intake();
    void Set_Intake(double intakeSpeed);
    double Get_Speed();
    void Reset_Point();
    double intake_distance = 0, start_point = 0;
    void Index_Intake(double speed, double distance);
    double Get_Position();
    void Create() override;
    void Stop() override;
    void Reset() override;
    std::string name = "Intake";

    enum Motor_Slot {Left = 0, Right};
protected:
    void Move_Motor() override;
  double Get_Real_Target();

    std::array<double, 2> _pid_inputs = {0,0};


    Motor_Controller *rightIntakeController;
    Motor_Controller *leftIntakeController;

    Pid *_master_intake_pid;
    double _intake_setpoint;
    double _outtake_setpoint;

    double _left_intake_value;
    double _right_intake_value;
    double _intake_value_average;
    double _master_intake_error_average;


};
#endif
