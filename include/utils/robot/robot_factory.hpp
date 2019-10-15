#ifndef ROBOT_FACTORY_H_
#define ROBOT_FACTORY_H_
#include <functional>
#include <map>
#include <string>
#include "utils/robot/robot.hpp"
#include "utils/robot/structure.hpp"
class RobotFactory {
	public:
	std::map<std::string, std::function<Structure*(void)>> factory_function_registry;

	Robot* create_robot(std::string drive_type = NULL,  std::string lift_type = NULL, std::string intake_type = NULL);
	void register_factory_function(std::string robot_type, std::function<Structure*(void)> class_factory_function);
	void assign_drive(std::string drive_type);
	static RobotFactory* instance();
};
#endif
