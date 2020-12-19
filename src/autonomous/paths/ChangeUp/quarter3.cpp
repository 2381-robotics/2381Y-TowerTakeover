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

AutoSequence *Auton::CUS_Q3 = AutoSequence::FromTasks({
    SingleRun([](void) -> void { position_tracker->Set_Position({0, -53}, -M_PI / 2); }),
    AutoCurve({0, -46}, -M_PI / 2 -0.2, {-10, -38}, -M_PI, 127, 3, 0.2)
        .AddRun([](void) -> void {
            indexer->Set_Indexer(100);
        }),

    AutoCurve({-10, -38}, -M_PI - 0.2, {-28, -46}, -M_PI - 0.2, 127, 3, 0)
        .AddRun([](void) -> void {
            intake->Set_Intake(100);
        }),
    AutoCurve({-28, -46}, -M_PI - 0.2, {-56, -42}, -5 * M_PI / 4 + 0.1, 127, 3)
        .AddRun([](void) -> void {
            intake->Set_Intake(127);
            indexer->Set_Indexer(127, true);
        }),

    AutoPath({-56, -42}, -5 * M_PI / 4 + 0.1, 127).AddRun([](void) -> void {
        intake->Set_Intake(127);
        indexer->Set_Indexer(127, true);
    }),
    AutoPath({-50, -52}, -3 * M_PI / 4, 127),
    AutoPath({-55.5,-55.5}, -3 * M_PI / 4, 127).AddRun([](void) -> void {
        intake->Set_Intake(0);
        indexer->Set_Indexer(127, true);
    }).TimeLimit(1000),
    AutoTask::AutoDelay(500).AddRun([](void) -> void {
                                intake->Set_Intake(0);
                                shooter->Shoot(127);
                            })
        .AddInit([](void) -> void { indexer->resetNewBall(); })
        .AddKill([](void) -> void { shooter->Set_Shooter(0); })
        .AddDone([](void) -> bool { return indexer->newBallIndexed(); }),
    AutoTask::AutoDelay(300),

    AutoCurve({-48, -48}, -3 * M_PI / 4, {-48, -12}, -3 * M_PI / 2, 127, 3).AddRun([](void) -> void {
        intake->Set_Intake(20);
        indexer->Set_Indexer(127, true);
    }),
    AutoPath({-48, -10}, -3 * M_PI/2, 127).AddRun([](void) -> void {
        intake->Set_Intake(127);
        indexer->Set_Indexer(127, true);
    }),
    AutoPath({-48, 0}, -M_PI, 200).AddRun([](void) -> void {
        intake->Set_Intake(0);
        indexer->Set_Indexer(0);
    }),
    AutoPath({-55, 0}, -M_PI, 127).AddRun([](void) -> void {
        intake->Set_Intake(0);
        indexer->Set_Indexer(50, true);
    }).TimeLimit(1000),

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
    AutoTask::AutoDelay(300),
    
    SingleRun([](void) -> void { position_tracker->Set_Position({0, 0}, 0, {-53, 0}, -M_PI); }),
});