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
        //     each tile is 24 inches, (0,0) at center of field, width of bot is 18, length is 14, tracked at center of bot, max distance is 3 tiles (72).  
        // autopath(AUTO_DRIVE.CPP) drives to a certain point P {0, -72}, and it will have the angle 0, and reach that point of 127     

    SingleRun([](void) -> void { position_tracker->Set_Position({36, 63}, -M_PI / 2); }),
        AutoTask::SyncTask(
        [](void) -> void {
            robot->drive->Set_Curve_Drive({48, 12}, -M_PI / 2, {48, 5}, 0, 150, 3);
            intake->Set_Intake(127);
            shooter->Set_Shooter(0);
            indexer->Set_Indexer(127, true);
        },
        [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void {}),

    AutoPath({48, 7}, 0, 200).AddRun([](void) -> void {
        intake->Set_Intake(127);
        indexer->Set_Indexer(127);
    }),
        // delay in ms
        AutoTask::AutoDelay(10000000),

        // set position is run only at the begining, and just sets position of robot
        // use this to run tasks that just need to run once and are immediete (not drive) 
        SingleRun([](void) -> void { position_tracker->Set_Position({36, 64.5}, -M_PI / 2); }),
        // autocurve (IM MECHDRIVE.CPP) interpolates 2 points curved towards waypoint, waypoint angle (angle it should be when closest to waypoint), endpoint, end angle, speed, curve 
        // add run run allows you to set speeds and values for all stuff other than drive (intake shooter and indexer)
        // does run while doing curve
        AutoCurve({36, 52}, -M_PI / 2, {58, 38}, 0, 127, 3).AddRun([](void) -> void {
                                                                   intake->Set_Intake(127);
                                                                   shooter->Set_Shooter(100);
                                                                   indexer->Set_Indexer(0);
                                                           })
        // runs at the end to 'reset' certain things 
        // intake is intake, shooter is to output balls, and indexer is to move balls up within the bot
            .AddKill([](void) -> void { shooter->Set_Shooter(0); }),

        AutoPath({59, 38}, 0, {127, 130}, 1).AddRun([](void) -> void {
                intake->Set_Intake(127);
                indexer->Set_Indexer(127, true);
        }),
        AutoPath({57.5, 57.5}, M_PI / 4, 127).AddRun([](void) -> void {
                intake->Set_Intake(0);
                indexer->Set_Indexer(127, true);
        }),
        AutoTask::AutoDelay(600).AddRun([](void) -> void {
                                        intake->Set_Intake(0);
                                        shooter->Shoot(127);
                                })
            .AddKill([](void) -> void { shooter->Shoot(0); indexer->Set_Indexer(0); })
        //     init is same as kill but it runs at the beggining of a task 
            .AddInit([](void) -> void { indexer->resetNewBall(); })
        //     done returns true or false and returns a boolean and only compeltes if that or the previous is true
            .AddDone([](void) -> bool { return indexer->newBallIndexed(); }),

        AutoCurve({48, 48}, M_PI/4, {48, 12}, -M_PI/2, 127,3).AddRun([](void) -> void {
                intake->Set_Intake(20);
                indexer->Set_Indexer(127, true);
        }),
        // wait for task to finish to run the next 
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
        SingleRun([](void) -> void { position_tracker->Set_Position({0, 0}, 0, {50, 1}, 0); }),
    });

AutoSequence *Auton::AT_Test_Ultras = AT_Test_Ultra1;

auto ree = AutoSequence::FromTasks(
    vector<AutoTask>{
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
