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
#include "autonomous/auto_drive.hpp"
#include "autonomous/global_sequences.hpp"

using namespace std;
using namespace Auton;  
using namespace pros;
AutoSequence *Auton::AT_Test_Ultras = AutoSequence::FromTasks(
    vector<AutoTask>{
AutoPath({30, 30}, 0),
        AutoTask::AutoDelay(1000),
AutoPath({0, 0}, M_PI/2),
        AutoTask::AutoDelay(1000),
        AutoPath({30, 30}, 0),
        AutoTask::AutoDelay(1000),
AutoPath({0, 0}, M_PI/2),
        AutoTask::AutoDelay(1000),
        AutoPath({30, 30}, 0),
        AutoTask::AutoDelay(1000),
AutoPath({0, 0}, M_PI/2),
        AutoTask::AutoDelay(1000),
        AutoPath({30, 30}, 0),
        AutoTask::AutoDelay(1000),
AutoPath({0, 0}, M_PI/2),
        AutoTask::AutoDelay(1000),

        // AutoTask::SyncTask(
        // [&](void) -> void {                         // Set Intake to max speed
        //     robot->drive->Set_Curve_Drive({0,0}, 0); // At the same time, drive forward towards the first row of cubes
        // },
        // [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void {}),

        // AutoTask::SyncTask(
        //     [](void) -> void {
        //         // robot->drive->Set_Point_Drive(60, 0, 3000, ultra_finder->Ultra_Angle(), 2, 0.5, false, 400, 1, {0, 0, 0, 0});
        //         // intake->Index_Intake(-50, 600);
        //         robot->drive->Set_Point_Drive(100, 0, 3000);

        //         // robot->drive->Set_Point_Drive(0,0, turnValue, 0);
        //     },
        //     [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void {}),
        // AutoTask::SyncTask(
        //     [](void) -> void {
        //         robot->drive->Set_Point_Drive(100, 180, 3000);

        //         // robot->drive->Set_Point_Drive(0,0, turnValue, 0);
        //     },
        //     [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void {}),

        AutoTask::AutoDelay(10000000),
    });
