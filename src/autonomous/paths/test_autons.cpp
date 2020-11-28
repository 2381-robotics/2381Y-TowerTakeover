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
        AutoPath({18, 48}, 0, {100, 150}, 1.5).AddRun([](void) -> void {
                intake->Set_Intake(100);
                shooter->Set_Shooter(100);
                indexer->Set_Indexer(0);
        }),
        AutoTask::SyncTask(
            [](void) -> void {
                    robot->drive->Set_Curve_Drive({48, 48}, 0, {57.5, 57.5}, M_PI / 4, 127, 3);
                    intake->Set_Intake(127);
                    shooter->Set_Shooter(0);
                    indexer->Set_Indexer(127, true);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void {}),
        AutoPath({57.5, 57.5}, M_PI/4, 127).AddRun([](void) -> void {
                intake->Set_Intake(0);
                indexer->Set_Indexer(127, true);
        }),
        AutoTask::AutoDelay(600).AddRun([](void) -> void {
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

        AutoTask::SyncTask(
            [](void) -> void {
                    robot->drive->Set_Curve_Drive({48, 12}, -M_PI / 2, {48, 1}, 0, 150, 3);
                    intake->Set_Intake(127);
                    shooter->Set_Shooter(0);
                    indexer->Set_Indexer(127, true);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void {}),

        // AutoPath({48, 4}, -M_PI / 2, 127, 2).AddRun([](void) -> void {
        //         intake->Set_Intake(100);
        //         indexer->Set_Indexer(100, true);
        // }),
        AutoPath({48, 1}, 0, 200).AddRun([](void) -> void {
                intake->Set_Intake(0);
                indexer->Set_Indexer(0);
        }),
        AutoPath({57, 1}, 0, 127).AddRun([](void) -> void {
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
        AutoPath({58, -30}, -M_PI / 4, 160, 3).AddRun([](void) -> void {
                intake->Set_Intake(127);
                shooter->Set_Shooter(0);
                indexer->Set_Indexer(100, true);
        }),
        AutoTask::SyncTask(
            [](void) -> void {
                    robot->drive->Set_Curve_Drive({54, -53}, -M_PI / 4 + 0.05, {58, -56}, -M_PI / 4 + 0.05, 160, 0.5);
                    intake->Set_Intake(127);
                    indexer->Set_Indexer(100, true);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void {}),

        AutoPath({58, -56}, -M_PI / 4 + 0.05, 160).AddRun([](void) -> void {
                intake->Set_Intake(0);
                shooter->Set_Shooter(0);
                indexer->Set_Indexer(100, true);
        }),
        AutoTask::AutoDelay(500).AddRun([](void) -> void {
                                        intake->Set_Intake(0);
                                        shooter->Shoot(127);
                                })
            .AddInit([](void) -> void { indexer->resetNewBall(); })
            .AddDone([](void) -> bool { return indexer->newBallIndexed(); }),

        AutoTask::SyncTask(
            [](void) -> void {
                    robot->drive->Set_Curve_Drive({60, -53}, -M_PI / 4 + 0.15, {56, -46}, -M_PI, 127, 3);
                    intake->Set_Intake(-30);
                    shooter->Set_Shooter(0);
                    indexer->Set_Indexer(100, true);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void {}),

        AutoPath({56, -46}, -M_PI, 180, {3, 5}).AddRun([](void) -> void {
                intake->Set_Intake(0);
                indexer->Set_Indexer(0);
        }),
        AutoTask::SyncTask(
            [](void) -> void {
                    robot->drive->Set_Curve_Drive({28, -46}, -M_PI, {0, -24}, -5 * M_PI / 4, 150, 3);
                    intake->Set_Intake(127);
                    shooter->Set_Shooter(0);
                    indexer->Set_Indexer(100, true);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void {}),

        AutoPath({0, -28}, -10 * M_PI / 8, {127, 150}, 2).AddRun([](void) -> void {
                intake->Set_Intake(127);
                indexer->Set_Indexer(127, true);
        }),
        AutoPath({2, -28}, -M_PI / 2, {127, 180}, 1),

        AutoPath({2, -56}, -M_PI / 2, {150, 150}, 1),

        AutoTask::AutoDelay(1000).AddRun([](void) -> void {
                                         intake->Set_Intake(0);
                                         shooter->Shoot(127);
                                         robot->drive->Set_Path_Drive({2, -56}, -M_PI / 2, 80);
                                 })
            .AddInit([](void) -> void {
                    indexer->resetNewBall();
                    robot->drive->Reset_Point();
            })
            .AddDone([](void) -> bool { return indexer->newBallIndexed(); }),
        AutoTask::AutoDelay(500).AddRun([](void) -> void {
                indexer->Set_Indexer(70, false);
                shooter->Shoot(0);
                robot->drive->Set_Path_Drive({2, -56}, -M_PI / 2, 80);
        }),
        AutoTask::AutoDelay(700).AddRun([](void) -> void {
                                        intake->Set_Intake(0);
                                        shooter->Shoot(127);
                                })
            .AddKill([](void) -> void { shooter->Shoot(0); indexer->Set_Indexer(0); })
            .AddInit([](void) -> void { indexer->resetNewBall(); })
            .AddDone([](void) -> bool { return indexer->newBallIndexed(); }),
        AutoTask::AutoDelay(10000000),
    });

AutoSequence *Auton::AT_Test_Ultras = AT_Test_Ultra1;


auto ree = AutoSequence::FromTasks(
    vector<AutoTask>{
        // SingleRun([](void) -> void { position_tracker->Set_Position({0, 0}, M_PI / 2); }),
        // AutoTask::SyncTask(
        //     [](void) -> void {
        //             robot->drive->Set_Curve_Drive({48, 48}, 0, {5, 57.5}, M_PI / 4, 127, 3);
        //             intake->Set_Intake(127);
        //             shooter->Set_Shooter(0);
        //             indexer->Set_Indexer(127, true);
        //     },
        //     [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void {}),
        // AutoPath({0,-24}, 0, 127),
        SingleRun([](void) -> void { position_tracker->Set_Position({48, 0}, 0); }),

        AutoPath({58, -30}, -M_PI / 4, 160, 3).AddRun([](void) -> void {
                intake->Set_Intake(127);
                shooter->Set_Shooter(0);
                indexer->Set_Indexer(100, true);
        }),
        AutoTask::SyncTask(
            [](void) -> void {
                    robot->drive->Set_Curve_Drive({54, -53}, -M_PI / 4 + 0.05, {58, -56}, -M_PI / 4 + 0.05, 160, 0.5);
                    intake->Set_Intake(127);
                    indexer->Set_Indexer(100, true);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void {}),

        AutoPath({58, -56}, -M_PI / 4 + 0.05, 160).AddRun([](void) -> void {
                intake->Set_Intake(0);
                shooter->Set_Shooter(0);
                indexer->Set_Indexer(100, true);
        }),
        AutoTask::AutoDelay(500).AddRun([](void) -> void {
                                        intake->Set_Intake(0);
                                        shooter->Shoot(127);
                                })
            .AddInit([](void) -> void { indexer->resetNewBall(); })
            .AddDone([](void) -> bool { return indexer->newBallIndexed(); }),

        AutoTask::SyncTask(
            [](void) -> void {
                    robot->drive->Set_Curve_Drive({60, -53}, -M_PI / 4 + 0.15, {56, -46}, -M_PI, 127, 3);
                    intake->Set_Intake(-30);
                    shooter->Set_Shooter(0);
                    indexer->Set_Indexer(100, true);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void {}),

        AutoPath({56, -46}, -M_PI, 180, {3, 5}).AddRun([](void) -> void {
                intake->Set_Intake(0);
                indexer->Set_Indexer(0);
        }),
        AutoTask::SyncTask(
            [](void) -> void {
                    robot->drive->Set_Curve_Drive({28, -46}, -M_PI, {0, -24}, -5 * M_PI / 4, 150, 3);
                    intake->Set_Intake(127);
                    shooter->Set_Shooter(0);
                    indexer->Set_Indexer(100, true);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void {}),

        AutoPath({0, -28}, -10 * M_PI / 8, {127, 150}, 2).AddRun([](void) -> void {
                intake->Set_Intake(127);
                indexer->Set_Indexer(127, true);
        }),
        AutoPath({2, -28}, -M_PI / 2, {127, 180}, 1),

        AutoPath({2, -56}, -M_PI / 2, {150, 150}, 1),

        AutoTask::AutoDelay(1000).AddRun([](void) -> void {
                                         intake->Set_Intake(0);
                                         shooter->Shoot(127);
                                         robot->drive->Set_Path_Drive({2, -56}, -M_PI / 2, 80);
                                 })
            .AddInit([](void) -> void {
                    indexer->resetNewBall();
                    robot->drive->Reset_Point();
            })
            .AddDone([](void) -> bool { return indexer->newBallIndexed(); }),
        AutoTask::AutoDelay(500).AddRun([](void) -> void {
                indexer->Set_Indexer(70, false);
                shooter->Shoot(0);
                robot->drive->Set_Path_Drive({2, -56}, -M_PI / 2, 80);
        }),
        AutoTask::AutoDelay(700).AddRun([](void) -> void {
                                        intake->Set_Intake(0);
                                        shooter->Shoot(127);
                                })
            .AddKill([](void) -> void { shooter->Shoot(0); indexer->Set_Indexer(0); })
            .AddInit([](void) -> void { indexer->resetNewBall(); })
            .AddDone([](void) -> bool { return indexer->newBallIndexed(); }),
        AutoTask::AutoDelay(10000000),
    });
