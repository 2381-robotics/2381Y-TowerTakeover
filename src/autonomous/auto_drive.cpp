#include "auto_drive.hpp"
#include <complex>
#include "auto_task.hpp"
#include "globals.hpp"

using namespace std;
AutoTask AutoPath(complex<double> EndPoint, double angle, double speed)
{
    return AutoTask::SyncTask(
        [EndPoint, angle, speed](void) -> void {                         
        // Lambda Capture By VALUE -> =, Lambda Capture by REFERENCE -> &
        // Default will capture here by Value because EndPoint is value not pointer.
        // If wanted to capture pointer of EndPoint, use &Endpoint.
        // Before, was trolling because [&] captures the pointer to EndPoint, but EndPoint is a function argument so probably
        // A temporary memory address that was deleted or overridden after, so it was capturing a pointer to garbage.
            robot->drive->Set_Curve_Drive(EndPoint, angle, speed);
            // robot->drive->Set_Path_Drive(EndPoint);
        },
        [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void {});
}