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
                robot->drive->Set_Point_Drive(60, 0, 3000, ultra_finder->Ultra_Angle(), 2, 0.5, false, 400, 1, {0, 0, 0, 0});
          
                // robot->drive->Set_Point_Drive(0,0, turnValue, 0);
            },
            [](void) -> bool { return (false); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
  
        AutoTask::AutoDelay(10000000),
    });
