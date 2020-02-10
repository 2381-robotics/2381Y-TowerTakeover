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

//5 point
AutoSequence *Auton::AT_Red5 = AutoSequence::FromTasks(
    vector<AutoTask>{
        // drive straight to reach the cubes
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(127);
                robot->drive->Set_Point_Drive(127, 0, 1000, 0, 1, 1, false, 400, 1, {0, 60, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        // slow downs while picking up cubes
        AutoTask::SyncTask(
            [](void) -> void {
                // intake->Set_Intake(127);
                robot->drive->Set_Point_Drive(60, 0, 2100);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),
        // extend angler a bit

        AutoTask::AutoDelay(200),
        // AutoTask::AsyncTask(
        //     [](void) -> void {
        //         angler->Smooth_Angler(1.8);
        //         intake->Set_Intake(-20);
        //     },
        //     [](void) -> bool { return ((angler->Get_Height() >= angler->_max_height / 3.2)); }),
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(40);
                robot->drive->Set_Point_Drive(127, 110, 2600, 110, 1, 1, false, 400, 1, {0, 70, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        AutoTask::SyncTask(
            [](void) -> void {
                robot->drive->Set_Point_Drive(127, 0, 500);
                intake->Set_Intake(-40);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        StackTask,
        AutoTask::AutoDelay(400, true, [](void) -> void {
            robot->set_drive(0, 40, 0, 0);
        }),
        AutoTask::SyncTask([](void) -> void {
                // intake->Set_Intake(-40);
                robot->drive->Set_Point_Drive(127, 180, 800); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),

        AutoTask::AutoDelay(10000000),
    });
