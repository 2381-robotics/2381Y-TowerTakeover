#include "main.h"
#include "robot/control/motor_controller.hpp"
#include "robot/modules/drive/mech_drive.hpp"
#include "robot/modules/drive/drive.hpp"

#include "api.h"
#include <math.h>
#include "robot/control/pid.hpp"
#include "globals.hpp"
#include "autonomous/auton_control.hpp"
#include "autonomous/auto_timer.hpp"
#include "autonomous/auto_sequence.hpp"
#include <vector>
#include <functional>
#include "autonomous/global_sequences.hpp"

using namespace std;
using namespace Auton;
using namespace pros;
AutoSequence *Auton::AT_Test_Ultras = AutoSequence::FromTasks(
    vector<AutoTask>{
        AutoTask::SyncTask(
            [](void) -> void {
                // robot->drive->Set_Point_Drive(127, 0, 3000, 0, 2, 0.5, false, 400, 1, {0, 0, 0, 0});
                double leftDistance = ultra_left.get_value();
                double rightDistance = ultra_right.get_value();
                lcd::set_text(2, "ultras - left" + to_string((int)leftDistance) + "right " + to_string((int)rightDistance));
                robot->drive->Set_Drive(0,0,(rightDistance-leftDistance)/(leftDistance + rightDistance), 0);
            },
            [](void) -> bool { return (false); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
  
        AutoTask::AutoDelay(10000000),
    });
