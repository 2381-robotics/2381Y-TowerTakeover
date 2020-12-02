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

AutoSequence *Auton::CUS_Q4 = AutoSequence::FromTasks({
    SingleRun([](void) -> void { position_tracker->Set_Position({-48, 0}, -M_PI); }),

    AutoPath({-48, 0}, M_PI / 2, 160),
    AutoCurve({-48, 48}, M_PI / 2, {-57.5, 57.5}, 3 * M_PI / 4, 127, 3),
    AutoPath({-57.5, 57.5}, 3 * M_PI / 4, 127),
    AutoTask::AutoDelay(500).AddRun([](void) -> void {
                                intake->Set_Intake(0);
                                shooter->Shoot(127);
                            })
        .AddInit([](void) -> void { indexer->resetNewBall(); })
        .AddKill([](void) -> void { shooter->Set_Shooter(0); })
        .AddDone([](void) -> bool { return indexer->newBallIndexed(); }),

    AutoCurve({-60, 53}, M_PI / 4 + 0.15, {-56, 48}, 0, 127, 3).AddRun([](void) -> void {
        intake->Set_Intake(-30);
        shooter->Set_Shooter(0);
        indexer->Set_Indexer(100, true);
    }),

    AutoPath({-56, 48}, 0, 180, {3, 5}).AddRun([](void) -> void {
        intake->Set_Intake(0);
        indexer->Set_Indexer(100);
    }),

    AutoCurve({-24, 48}, 0, {0, 57.5}, M_PI / 2, 127, 3).AddRun([](void) -> void {
        intake->Set_Intake(127);
        shooter->Set_Shooter(0);
        indexer->Set_Indexer(127, true);
    }),

    AutoPath({0, 57.5}, M_PI / 2, 127).AddRun([](void) -> void {
        intake->Set_Intake(0);
        indexer->Set_Indexer(100, true);
    }),

    AutoTask::AutoDelay(500).AddRun([](void) -> void {
                                intake->Set_Intake(0);
                                shooter->Shoot(127);
                            })
        .AddInit([](void) -> void { indexer->resetNewBall(); })
        .AddKill([](void) -> void { shooter->Set_Shooter(0); })
        .AddDone([](void) -> bool { return indexer->newBallIndexed(); }),

    AutoPath({0, 48}, M_PI / 2, 127, 4).AddRun([](void) -> void {
        intake->Set_Intake(0);
        indexer->Set_Indexer(0, true);
    }),

    AutoPath({0, 48}, -M_PI / 2, 127).AddRun([](void) -> void {
        intake->Set_Intake(0);
        indexer->Set_Indexer(0, true);
    }),

    AutoPath({0, 10}, -M_PI / 2, 127).AddRun([](void) -> void {
        intake->Set_Intake(127);
        indexer->Set_Indexer(100, true);
    }),
    AutoTask::AutoDelay(1000).AddRun([](void) -> void {
        intake->Set_Intake(127);
        indexer->Set_Indexer(50, true);
    }),

    AutoTask::AutoDelay(500).AddRun([](void) -> void {
                                intake->Set_Intake(0);
                                shooter->Shoot(127);
                            })
        .AddInit([](void) -> void { indexer->resetNewBall(); })
        .AddKill([](void) -> void { shooter->Set_Shooter(0); indexer->Set_Indexer(0); })
        .AddDone([](void) -> bool { return indexer->newBallIndexed(); }),

    AutoTask::AutoDelay(2000).AddRun([](void) -> void {
        intake->Set_Intake(127);
        indexer->Set_Indexer(127, true);
    }),
    
    AutoPath({0, 48}, -M_PI / 2, 127, 4).AddRun([](void) -> void {
        intake->Set_Intake(0);
        indexer->Set_Indexer(100, true);
    }),

    SingleRun([](void)-> void {STOP = true;})
});