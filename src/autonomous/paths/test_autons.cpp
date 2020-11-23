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
AutoSequence *AT_Test_Ultra1 = AutoSequence::FromTasks(
    vector<AutoTask>{
        SingleRun([](void) -> void { position_tracker->Set_Position({7, 64.5}, -M_PI / 2); }),
        AutoPath({18, 48}, 0, 127, 3).AddRun([](void) -> void {
                intake->Set_Intake(100);
                shooter->Set_Shooter(100);
                indexer->Set_Indexer(0);
        }),
        AutoPath({48, 48}, 0, 127).AddRun([](void) -> void {
                intake->Set_Intake(127);
                shooter->Set_Shooter(0);

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
        AutoPath({48, 48}, M_PI / 4, 127, 2).AddRun([](void) -> void { intake->Set_Intake(-30); }),
        AutoPath({48, 48}, -M_PI / 4 + 0.3, 150),
        AutoPath({58, 42}, -M_PI / 4 + 0.3, 127).AddRun([](void) -> void {
                intake->Set_Intake(127);
                indexer->Set_Indexer(100, true);
        }),
        AutoPath({48, 30}, -M_PI / 2, 127, 3).AddRun([](void) -> void {
                intake->Set_Intake(100);
                indexer->Set_Indexer(100, true);
        }),
        AutoPath({48, 4}, -M_PI / 2, 127, 2).AddRun([](void) -> void {
                intake->Set_Intake(100);
                indexer->Set_Indexer(100, true);
        }),
        AutoPath({48, 1}, 0, 200).AddRun([](void) -> void {
                intake->Set_Intake(0);
                indexer->Set_Indexer(0);
        }),
        AutoPath({57, 1}, 0, 100).AddRun([](void) -> void {
                intake->Set_Intake(0);
                indexer->Set_Indexer(50, true);
        }),
        AutoTask::AutoDelay(1000).AddRun([](void) -> void {
                                         intake->Set_Intake(0);
                                         shooter->Shoot(127);
                                 })
            .AddInit([](void) -> void { indexer->resetNewBall(); })
            .AddDone([](void) -> bool { return indexer->newBallIndexed(); }),
        AutoTask::AutoDelay(500).AddRun([](void) -> void {indexer->Set_Indexer(70, false); shooter->Shoot(0); }),
        AutoTask::AutoDelay(700).AddRun([](void) -> void {
                                        intake->Set_Intake(0);
                                        shooter->Shoot(127);
                                })
            .AddKill([](void) -> void { shooter->Shoot(0); indexer->Set_Indexer(0); })
            .AddInit([](void) -> void { indexer->resetNewBall(); })
            .AddDone([](void) -> bool { return indexer->newBallIndexed(); }),

        AutoPath({50, 1}, 0, 127).AddRun([](void) -> void {
                intake->Set_Intake(-20);
                indexer->Set_Indexer(0);
        }),
        SingleRun([](void) -> void { position_tracker->Set_Position({48, 0}, 0, {50, 1}, 0); }),

        AutoTask::AutoDelay(10000000),
    });




AutoSequence *Auton::AT_Test_Ultras = AutoSequence::FromTasks(
    vector<AutoTask>{
            AutoTask::AutoDelay(1000).AddRun([](void) -> void {
                                         intake->Set_Intake(0);
                                         shooter->Shoot(127);
                                 })
            .AddInit([](void) -> void { indexer->resetNewBall(); })
            .AddDone([](void) -> bool { return indexer->newBallIndexed(); }),
        AutoTask::AutoDelay(500).AddRun([](void) -> void {indexer->Set_Indexer(70, false); shooter->Shoot(0); }),
        AutoTask::AutoDelay(700).AddRun([](void) -> void {
                                        intake->Set_Intake(0);
                                        shooter->Shoot(127);
                                })
            .AddKill([](void) -> void { shooter->Shoot(0); indexer->Set_Indexer(0); })
            .AddInit([](void) -> void { indexer->resetNewBall(); })
            .AddDone([](void) -> bool { return indexer->newBallIndexed(); }),
        AutoTask::AutoDelay(10000000),

        SingleRun([](void) -> void { position_tracker->Set_Position({48, 0}, 0); }),
        AutoPath({58, -30}, -M_PI / 4, 160).AddRun([](void) -> void {
                intake->Set_Intake(127);
                shooter->Set_Shooter(0);
                indexer->Set_Indexer(100, true);
        }),
        AutoPath({56, -56},-M_PI / 4, 160).AddRun([](void) -> void {
                intake->Set_Intake(0);
                shooter->Set_Shooter(0);
                indexer->Set_Indexer(100, true);
        }),
        AutoTask::AutoDelay(1000).AddRun([](void) -> void {
                                         intake->Set_Intake(0);
                                         shooter->Shoot(127);
                                 })
            .AddInit([](void) -> void { indexer->resetNewBall(); })
            .AddDone([](void) -> bool { return indexer->newBallIndexed(); }),
        
        AutoPath({52, -48}, -M_PI / 4 + 0.3, 127, 3).AddRun([](void) -> void {
                intake->Set_Intake(-30);
                shooter->Set_Shooter(0);
                indexer->Set_Indexer(100, true);
        }),
        AutoPath({52, -48}, -M_PI , 200).AddRun([](void) -> void {
                intake->Set_Intake(-30);
                shooter->Set_Shooter(0);
                indexer->Set_Indexer(100, true);
        }),
        AutoTask::AutoDelay(10000000),
    });
