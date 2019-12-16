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
    std::map<std::string, AutoSequence*> autonomous_registry;
    void define_auton(std::string auton_name, AutoSequence* auton);

    void select_auton(std::string auton);
    static AutonControl* instance();
    void run();
private:
    AutoSequence* _auton_running;
 
};
#endif
