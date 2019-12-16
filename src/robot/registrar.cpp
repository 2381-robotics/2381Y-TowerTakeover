#include "registrar.hpp"
#include "robot.hpp"
#include "robot_factory.hpp"
#include "modules/drive/mech_drive.hpp"
#include "structure.hpp"
#include "modules/lift/Y_lift.hpp"
template <class T>
Registrar<T>::Registrar(std::string robot_type) {
	// register the class factory function
	RobotFactory::instance()->register_factory_function(robot_type, [](void) -> Structure* { return new T(); });
}

static Registrar<Mech_Drive> registrar_drive_mech("Mech_Drive");
static Registrar<Y_Lift> registrar_lift_Y("Y_Lift");
static Registrar<Robot> registrar_robot("ROBOT_MAIN");
