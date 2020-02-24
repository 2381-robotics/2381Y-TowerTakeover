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

AutoTask Auton::DeployTask() { return AutoTask::AutoDelay(
    900, true, [](void) -> void {
                                arm->Set_Target(300);
                                robot->drive->Set_Drive(0,-30,0,0);
                                intake->Set_Intake(-60); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Set_Intake(127); arm->Set_Target(0); });}
                                ;



AutoTask Auton::ArmTask()
{
    return AutoTask::SyncTask(

        [](void) -> void {
            // robot->drive->Set_Point_Drive(20, 180, 200);
            arm->Increment_Arm(1);
        },
        [](void) -> bool { return ((arm->Get_Height() >= 1000));}
    );
};



AutoTask Auton::StackTask()
{
    return AutoTask::SyncTask(
        [](void) -> void {
            // intake->Set_Intake(0);
            robot->set_drive(0, 0, 0, 0);
            angler->Smooth_Angler(1);
            angler->Override_Mode(-1);
        },
        [](void) -> bool { return ((angler->Get_Height() >= angler->_max_height-100)); },
        [](void) -> void {
            angler->Override_Mode(1);
        },
        [](void) -> void {
            angler->Override_Mode(0);
        });
};
AutoTask stackTask2 = AutoTask::SyncTask(
    [](void) -> void {
        // intake->Set_Intake(0);
        // robot->set_drive(-30, 40, 0, 0);
        // angler->Smooth_Angler_Auton(1);
        robot->drive->Set_Point_Drive(35, 0, 625);
        intake->Set_Intake(-10);
        // intake->Set_Intake(45);
        // robot->drive->Move_Wheel(30);
        // robot->drive->Set_Point_Drive(30, 0, 1500);
        // robot->set_drive(40, 50, 0, 0);
    },
    [](void) -> bool { return ((angler->Get_Height() >= angler->_max_height / 2.5) && (abs(angler->Get_Speed() < 20))); });

AutoTask Turn90DegRight = AutoTask::SyncTask(
    [](void) -> void {
        // intake->Set_Intake(127);
        robot->drive->Set_Point_Drive(0, 0, 1700, 127, 1.5, 0.8);
    },
    [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); });

AutoTask Turn90DegLeft = AutoTask::SyncTask(
    [](void) -> void {
        // intake->Set_Intake(127);
        robot->drive->Set_Point_Drive(0, 0, 1700, -127, 1.5, 0.8);
    },
    [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); });

AutoTask Turn90Deg = AutoTask::SyncTask(
    [](void) -> void {
        intake->Set_Intake(127);
        robot->drive->Set_Point_Drive(0, 0, 1290, 100, 2.5);
  
    },
    [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); robot->drive->Stop(); });

// acc like 55 degrees
AutoTask Turn45Deg = AutoTask::SyncTask(
    [](void) -> void {
        intake->Set_Intake(127);
        // 625
        robot->drive->Set_Point_Drive(0, 0, 710 + autonomous_increment, 100, 2.5);
  
    },
    [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); });

AutoTask InvertTurn45Deg = AutoTask::SyncTask(
    [](void) -> void {
        intake->Set_Intake(127);
        // 625
        robot->drive->Set_Point_Drive(0, 0, 700 + autonomous_increment, -75, 2.5);
  
    },
    [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); });

AutoTask Auton::InvertTurn90Deg() {
    return AutoTask::SyncTask(
    [](void) -> void {
        intake->Set_Intake(127);
        robot->drive->Set_Point_Drive(0, 0, 1295 + 650 + autonomous_increment, -90, 2.5);

  
    },
    [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); });
};
