#include "shooter.hpp"
#include "api.h"
#include "main.h"
#include "globals.hpp"
#include <array>

using namespace std;

double Shooter::Get_Speed()
{
    return (shooterMC->Get_Speed());
}

void Shooter::Shoot(double speed)
{
    Set_Shooter(speed);
    if(speed != 0 )
    {
        indexer->Set_Indexer(speed, true);
    }
}

void Shooter::Move_Motor()
{
    if (_pid_input == 0)
    {
        shooter_input_value = shooterMC->Set_Speed(0);
    }
    else
    {
        shootMotor.move_voltage(_pid_input / 127 * 12000);
    }
}
void Shooter::Set_Shooter(double shooterSpeed)
{
    _pid_input = shooterSpeed;
}
//Empty default constructor for blank factory arguments.
Shooter::Shooter(){};

void Shooter::Reset()
{
    Stop();
}

double Shooter::Get_Position()
{
    return (shootMotor.get_position());
}
void Shooter::Reset_Point()
{
    start_point = Get_Position();
}

void Shooter::Stop()
{
    shootMotor.move_voltage(0);
}

void Shooter::Create()
{
    shooterMC = new Motor_Controller(pid_shooter_values[0], pid_shooter_values[1], pid_shooter_values[2], &shootMotor);
}
