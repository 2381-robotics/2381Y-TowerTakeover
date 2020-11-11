#include "auto_drive.hpp"
#include <complex>
#include "auto_task.hpp"
#include "globals.hpp"

using namespace std;
AutoTask AutoPath(complex<double> EndPoint, double angle)
{ 
    return AutoTask::SyncTask(
        [&](void) -> void {                         // Set Intake to max speed
            robot->drive->Set_Path_Drive({-30,30}); // At the same time, drive forward towards the first row of cubes
        },
        [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void {});
}