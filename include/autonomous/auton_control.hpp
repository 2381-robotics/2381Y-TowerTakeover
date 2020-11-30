#ifndef AUTON_CONTROL_HPP
#define AUTON_CONTROL_HPP
#include "robot/modules/drive/drive.hpp"
#include "autonomous/auto_sequence.hpp"

#include <array>
#include <tuple>
#include <map>

class AutonControl
{
public:
    enum Autonomous
    {
        RedSmallSideAuton = 0,
        BlueSmallSideAuton,
        Blue5PointAuton,
        Red5PointAuton,
        Blue7PointAuton,
        Red7PointAuton,
        SkillsAuton,
        TestAuton,



        CUS_ALL,
        CUS_Q1,
        CUS_Q2,
        CUS_Q3,
        CUS_Q4,
    };

    std::map<Autonomous, AutoSequence *> autonomous_registry;
    int define_auton(Autonomous sequence, AutoSequence *auton);

    void select_auton(Autonomous sequence);
    static AutonControl *instance();
    void run();
    void Reset();

private:
    AutoSequence *_auton_running;
};
#endif
