#include "robot_factory.hpp"
#include "robot.hpp"
#include "structure.hpp"
Robot* RobotFactory::create_robot(std::string drive_type, std::string lift_type, std::string intake_type) {
	Robot* instance = nullptr;
	auto it = factory_function_registry.find("ROBOT_MAIN");
	if (it != factory_function_registry.end()) {
		// mapped value (std::function<Robot*(void)>).
		instance = dynamic_cast<Robot*>(it->second());
	}

	if (instance != nullptr) {
		auto drive_instance = factory_function_registry.find(drive_type);
		if (drive_instance != factory_function_registry.end()) {
			// mapped value (std::function<Robot*(void)>).
			instance->assign_drive(dynamic_cast<Drive*>(drive_instance->second()));
		}
		auto lift_instance = factory_function_registry.find(lift_type);
		if (lift_instance != factory_function_registry.end()) {
			// mapped value (std::function<Robot*(void)>).
			instance->assign_lift(lift_instance->second());
		}
		auto intake_instance = factory_function_registry.find(intake_type);
		if (intake_instance != factory_function_registry.end()) {
			// mapped value (std::function<Robot*(void)>).
			instance->assign_lift(intake_instance->second());
		}
		return instance;
	} else {
		return nullptr;
	}
}

void RobotFactory::register_factory_function(std::string robot_type,
                                             std::function<Structure*(void)> class_factory_function) {
	factory_function_registry[robot_type] = class_factory_function;
}

RobotFactory* RobotFactory::instance() {
	static RobotFactory factory;
	return &factory;
}
