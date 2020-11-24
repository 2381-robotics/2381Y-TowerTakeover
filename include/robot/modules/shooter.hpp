#ifndef SHOOTER_HPP
#define SHOOTER_HPP

#include "robot/control/motor_controller.hpp"
#include <array>
#include "robot/module.hpp"

class Shooter : public Module
{
public:
    Shooter();

    void Shoot(double speed);
    void Set_Shooter(double shooterSpeed);
    double Get_Speed();
    void Reset_Point();
    double intake_distance = 0, start_point = 0;

    double Get_Position();
    void Stop() override;
    void Reset() override;
    std::string name = "Shooter";
    
protected:

    void Create() override;
    void Move_Motor() override;
    double Get_Real_Target();
    
    double _pid_input = 0, shooter_input_value = 0;
    Motor_Controller *shooterMC;

};
#endif
