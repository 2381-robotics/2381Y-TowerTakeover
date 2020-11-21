#ifndef INDEXER_HPP
#define INDEXER_HPP

#include "robot/control/motor_controller.hpp"
#include <array>
#include "robot/module.hpp"

class Indexer : public Module
{
public:
    Indexer();

    void Set_Indexer(double speed, bool override = false);
    bool IsBallIndexed();
    bool newBallIndexed();
    void resetNewBall();

    double Get_Speed();
    void Reset_Point();
    double start_point = 0;

    double Get_Position();
    void Create() override;
    void Stop() override;
    void Reset() override;
    std::string name = "Indexer";
protected:
    void Move_Motor() override;
    double Get_Real_Target();
    
    double _pid_input = 0, indexer_input_value = 0;
    Motor_Controller *indexerMC;
};
#endif
