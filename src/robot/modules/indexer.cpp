#include <array>

#include "indexer.hpp"
#include "api.h"
#include "main.h"
#include "globals.hpp"
#include "robot/sensors/light_indexer.hpp"

using namespace std;

double Indexer::Get_Speed()
{
    return (indexerMC->Get_Speed());
}

void Indexer::Move_Motor()
{
    if (_pid_input == 0 )
    {
        indexer_input_value = indexerMC->Set_Speed(0);
    }
    else
    {
        indexMotor.move_voltage(_pid_input / 127 * 12000);
    }
}
void Indexer::Set_Indexer(double indexerSpeed, bool override)
{
    if(IsBallIndexed() && !override && indexerSpeed > 0)
    {
        _pid_input = 0;
    } else
    {
        _pid_input = indexerSpeed;
    }
}

bool Indexer::IsBallIndexed()
{
    auto indexValue = light_indexer->Get_Value();
    pros::lcd::set_text(4, "LINE TRACK:" + to_string(indexValue));
    return indexValue < 2000;
}

auto previousIndexed = false;
auto newBall = false;

bool Indexer::newBallIndexed()
{
    newBall = newBall || (IsBallIndexed() && !previousIndexed);
    previousIndexed = IsBallIndexed();
    return newBall;
}

void Indexer::resetNewBall()
{
    previousIndexed = IsBallIndexed();
    newBall = false;
}

//Empty default constructor for blank factory arguments.
Indexer::Indexer(){};

void Indexer::Reset()
{
    Stop();
}

double Indexer::Get_Position()
{
    return (shootMotor.get_position());
}
void Indexer::Reset_Point()
{
    start_point = Get_Position();
}

void Indexer::Stop()
{
    shootMotor.move_voltage(0);
}

void Indexer::Create()
{
    indexerMC = new Motor_Controller(pid_indexer_values[0], pid_indexer_values[1], pid_indexer_values[2], &indexMotor);
}
