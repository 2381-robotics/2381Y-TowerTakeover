#include <math.h>
#include <vector>
#include <functional>

#include "main.h"

#include "api.h"
#include "globals.hpp"
#include "autonomous/auton_control.hpp"
#include "autonomous/auto_timer.hpp"
#include "autonomous/auto_sequence.hpp"
#include "autonomous/auto_drive.hpp"
#include "autonomous/global_sequences.hpp"

using namespace std;
using namespace Auton;
using namespace pros;

AutoSequence *Auton::CUS_Q1 = AutoSequence::FromTasks({
    // Start Position

    // setting initial position of the bot
    SingleRun([](void) -> void { position_tracker->Set_Position({36, 63}, -M_PI / 2); }),


    // Curves the both towards left to pick up ball, also sets intake to intake a second ball infront, sets shooter to deploy roof, and runs indexer
    AutoCurve({36, 52}, -M_PI / 2, {59, 38}, 0, 127, 3)
        .AddRun([](void) -> void {
            intake->Set_Intake(127);
            shooter->Set_Shooter(100);
            indexer->Set_Indexer(20);
        })
        // resetting shooter
        .AddKill([](void) -> void { shooter->Set_Shooter(0); }),

    // moving towards bottom left tower and intaking and moving balls up at the same time
    AutoPath({57, 38}, 0, {127, 130}, 1).AddRun([](void) -> void {
        intake->Set_Intake(127);
        // light sensor (true or false), false will make it stop running as soon as it detects it
        indexer->Set_Indexer(127, true);
    }),

    AutoCurve({48, 48}, M_PI/4, {56, 56}, M_PI/4, 127, 3),
    AutoPath({56, 56}, M_PI / 4, 127).AddRun([](void) -> void {
        intake->Set_Intake(20);
        indexer->Set_Indexer(127, true);
    }),

    AutoTask::AutoDelay(600)
        .AddRun([](void) -> void {
            intake->Set_Intake(0);
            shooter->Shoot(127);
        })
        .AddKill([](void) -> void { shooter->Shoot(0); indexer->Set_Indexer(0); })
        .AddInit([](void) -> void { indexer->resetNewBall(); })
        .AddDone([](void) -> bool { return indexer->newBallIndexed(); }),

    AutoCurve({48, 48}, M_PI / 4, {48, 12}, -M_PI / 2, 127, 3).AddRun([](void) -> void {
        intake->Set_Intake(20);
        indexer->Set_Indexer(127, true);
    }),

    AutoTask::SyncTask(
        [](void) -> void {
            robot->drive->Set_Curve_Drive({48, 12}, -M_PI / 2, {48, 5}, 0, 150, 3);
            intake->Set_Intake(127);
            shooter->Set_Shooter(0);
            indexer->Set_Indexer(127, true);
        },
        [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void {}),

    // AutoPath({48, 4}, -M_PI / 2, 127, 2).AddRun([](void) -> void {
    //         intake->Set_Intake(100);
    //         indexer->Set_Indexer(100, true);
    // }),
    AutoPath({48, 8}, 0, 200).AddRun([](void) -> void {
        intake->Set_Intake(127);
        indexer->Set_Indexer(127);
    }),

    AutoPath({55, 8}, 0, 127).AddRun([](void) -> void {
        intake->Set_Intake(0);
        indexer->Set_Indexer(50, true);
    }),

    AutoTask::AutoDelay(1000)
        .AddRun([](void) -> void {
            intake->Set_Intake(0);
            shooter->Shoot(127);
        })
        .AddInit([](void) -> void { indexer->resetNewBall(); })
        .AddDone([](void) -> bool { return indexer->newBallIndexed(); }),

    AutoTask::AutoDelay(500).AddRun([](void) -> void {indexer->Set_Indexer(70, false); shooter->Shoot(0); }),

    AutoTask::AutoDelay(700)
        .AddRun([](void) -> void {
            intake->Set_Intake(0);
            shooter->Shoot(127);
        })
        .AddKill([](void) -> void { shooter->Shoot(0); indexer->Set_Indexer(0); })
        .AddInit([](void) -> void { indexer->resetNewBall(); })
        .AddDone([](void) -> bool { return indexer->newBallIndexed(); }),

    AutoPath({48, 8}, 0, 127).AddRun([](void) -> void {
        intake->Set_Intake(-20);
        indexer->Set_Indexer(0);
    }),
    SingleRun([](void) -> void { position_tracker->Set_Position({0, 0}, 0, {48, 8}, 0); })
});
