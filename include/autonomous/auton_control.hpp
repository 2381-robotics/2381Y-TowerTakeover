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
    enum Autonomous {RedSmallSideAuton = 0, BlueSmallSideAuton};


    std::map<Autonomous, AutoSequence*> autonomous_registry;
    void define_auton(Autonomous sequence, AutoSequence* auton);

    void select_auton(Autonomous sequence);
    static AutonControl* instance();
    void run();
private:
    AutoSequence* _auton_running;
 
};
#endif
