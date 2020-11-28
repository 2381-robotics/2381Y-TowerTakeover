#include <array>

#include <tuple>
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
        indexer_motor->move_voltage(_pid_input / 127 * 12000);
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
    // pros::lcd::set_text(4, "LINE TRACK:" + to_string(indexValue));
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
Indexer::Indexer(std::tuple<uint8_t, bool> motor_config) : motor_config_(motor_config_) {

};

void Indexer::Reset()
{
    Stop();
}

double Indexer::Get_Position()
{
    return (indexer_motor->get_position());
}
void Indexer::Reset_Point()
{
    start_point = Get_Position();
}

void Indexer::Stop()
{
    indexer_motor->move_voltage(0);
}

void Indexer::Create()
{
    indexer_motor = new pros::Motor(INDEXER_MOTOR_PORT, INDEXER_MOTOR_ORIENTATION);
    indexerMC = new Motor_Controller(indexer_pid_config[0], indexer_pid_config[1], indexer_pid_config[2], indexer_motor);
}
