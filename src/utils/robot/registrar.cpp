#include "registrar.hpp"
#include "robot.hpp"
#include "robot_factory.hpp"
#include "drive/mech_drive.hpp"
#include "structure.hpp"
template <class T>
Registrar<T>::Registrar(std::string robot_type) {
	// register the class factory function
	RobotFactory::instance()->register_factory_function(robot_type, [](void) -> Structure* { return new T(); });
}

static Registrar<Mech_Drive> registrar_drive_mech("MechanumDrive");
static Registrar<Robot> registrar_robot("ROBOT_MAIN");
