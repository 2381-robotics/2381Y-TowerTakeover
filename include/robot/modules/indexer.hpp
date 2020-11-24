#ifndef INDEXER_HPP
#define INDEXER_HPP

#include <tuple>
#include <array>
#include "robot/control/motor_controller.hpp"
#include "robot/module.hpp"

class Indexer : public Module
{
public:
    Indexer(std::tuple<uint8_t, bool> motor_config);

    void Set_Indexer(double speed, bool override = false);
    bool IsBallIndexed();
    bool newBallIndexed();
    void resetNewBall();

    double Get_Speed();
    void Reset_Point();
    double start_point = 0;

    double Get_Position();
    void Stop() override;
    void Reset() override;
    
    std::string name = "Indexer";
    static std::array<double, 3> indexer_pid_config;
protected:

    std::tuple<uint8_t, bool> motor_config_;
    pros::Motor * indexer_motor = nullptr;

    void Create() override;
    void Move_Motor() override;
    double Get_Real_Target();
    
    double _pid_input = 0, indexer_input_value = 0;
    Motor_Controller *indexerMC;
};
#endif
