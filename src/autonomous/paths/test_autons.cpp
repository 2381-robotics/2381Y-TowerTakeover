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

        AutoPath({18, 48}, 0, 127, 3).AddRun([](void) -> void {
                intake->Set_Intake(0);
                shooter->Shoot(0);
        }),
        AutoPath({48, 48}, 0, 127).AddRun([](void) -> void {
                intake->Set_Intake(127);
                indexer->Set_Indexer(127, true);
        }),
        AutoPath({58.5, 58.5}, M_PI / 4, 127).AddRun([](void) -> void {
                intake->Set_Intake(60);
                indexer->Set_Indexer(127, true);
        }),
        AutoTask::AutoDelay(1000).AddRun([](void) -> void {
                                         intake->Set_Intake(0);
                                         shooter->Shoot(127);
                                 })
            .AddKill([](void) -> void { shooter->Shoot(0); indexer->Set_Indexer(0); })
            .AddInit([](void) -> void { indexer->resetNewBall(); })
            .AddDone([](void) -> bool { return indexer->newBallIndexed(); }),
        AutoPath({48, 48}, M_PI / 4, 127, 2).AddRun([](void) -> void { intake->Set_Intake(-60); }),
        AutoPath({48, 48}, -M_PI / 4 + 0.3, 150),
        AutoPath({61, 45}, -M_PI / 4 + 0.3, 127).AddRun([](void) -> void {
                intake->Set_Intake(127);
                indexer->Set_Indexer(100, true);
        }),
        AutoPath({48, 30}, -M_PI / 2, 127, 3).AddRun([](void) -> void {
                intake->Set_Intake(100);
                indexer->Set_Indexer(100, true);
        }),
        AutoPath({48, 4}, -M_PI / 2, 127 , 2).AddRun([](void) -> void {
                intake->Set_Intake(100);
                indexer->Set_Indexer(100, true);
        }),
        AutoPath({48, 1}, 0, 200).AddRun([](void) -> void {
                intake->Set_Intake(0);
                indexer->Set_Indexer(0);
        }),
        AutoPath({57, 1}, 0, 100).AddRun([](void) -> void {
                intake->Set_Intake(0);
                indexer->Set_Indexer(0);
        }),
        AutoTask::AutoDelay(1000).AddRun([](void) -> void {
                                         intake->Set_Intake(0);
                                         shooter->Shoot(127);
                                 })
            .AddInit([](void) -> void { indexer->resetNewBall(); })
            .AddDone([](void) -> bool { return indexer->newBallIndexed(); }),
        AutoTask::AutoDelay(500).AddRun([](void)-> void {indexer->Set_Indexer(70, false); shooter->Shoot(0);}),
        AutoTask::AutoDelay(700).AddRun([](void) -> void {
                                         intake->Set_Intake(0);
                                         shooter->Shoot(127);
                                 })
            .AddKill([](void) -> void { shooter->Shoot(0); indexer->Set_Indexer(0); })
            .AddInit([](void) -> void { indexer->resetNewBall(); })
            .AddDone([](void) -> bool { return indexer->newBallIndexed(); }),

        AutoPath({52, 1}, 0, 127).AddRun([](void) -> void {
                intake->Set_Intake(0);
                indexer->Set_Indexer(0);
        }),
        // AutoPath({48, 0}, -M_PI/2, 100).AddRun([](void)-> void {
        //         intake->Set_Intake(0);
        // }),

        AutoTask::AutoDelay(10000000),

        AutoPath({-24, 24}, M_PI, 100),
        AutoTask::AutoDelay(1000),
        AutoPath({0, 0}, M_PI / 2, 100),
        AutoTask::AutoDelay(1000),
        AutoPath({-24, 24}, M_PI, 100),
        AutoTask::AutoDelay(1000),
        AutoPath({0, 0}, M_PI / 2, 100),
        AutoTask::AutoDelay(1000),
        AutoPath({-24, 24}, M_PI, 100),
        AutoTask::AutoDelay(1000),
        AutoPath({0, 0}, M_PI / 2, 100),
        AutoTask::AutoDelay(1000),
        AutoPath({-24, 24}, M_PI, 100),
        AutoTask::AutoDelay(1000),
        AutoPath({0, 0}, M_PI / 2, 100),
        AutoTask::AutoDelay(1000),

        // AutoPath({24, 24}, 0, 100),
        //         AutoTask::AutoDelay(1000),
        // AutoPath({0, 0}, M_PI/2, 100),
        //         AutoTask::AutoDelay(1000),
        //         AutoPath({24, 24}, 0, 100),
        //         AutoTask::AutoDelay(1000),
        // AutoPath({0, 0}, M_PI/2, 100),
        //         AutoTask::AutoDelay(1000),
        //         AutoPath({24, 24}, 0, 100),
        //         AutoTask::AutoDelay(1000),
        // AutoPath({0, 0}, M_PI/2, 100),
        //         AutoTask::AutoDelay(1000),
        //         AutoPath({24, 24}, 0, 100),
        //         AutoTask::AutoDelay(1000),
        // AutoPath({0, 0}, M_PI/2, 100),
        //         AutoTask::AutoDelay(1000),

        // AutoTask::SyncTask(
        // [&](void) -> void {                         // Set Intake to max speed
        //     robot->drive->Set_Curve_Drive({0,0}, 0); // At the same time, drive forward towards the first row of cubes
        // },
        // [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void {}),

        // AutoTask::SyncTask(
        // [](void) -> void {
        //         robot->drive->Set_Point_Drive(100, 1100, 3000);

        //         // robot->drive->Set_Point_Drive(0,0, turnValue, 0);
        //     },
        //     [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void {}),

        AutoTask::AutoDelay(10000000),
    });
