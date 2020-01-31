/**
 * \file robot.hpp
 * \brief
 */
#ifndef ROBOT_HPP_
#define ROBOT_HPP_
#include "robot/modules/drive/drive.hpp"
#include "robot/modules/drive/mech_drive.hpp"

#include "robot/structure.hpp"
#include "robot/modules/lift/lift.hpp"
#include <map>
/**
 * \class Robot
 * \brief A singleton thread safe robot class
 */
class Robot : public Structure{
	public:
	Robot();
	enum class DriveType;
	enum class DrivingStyle;

	virtual ~Robot();

	void Create() override;

	void begin_tasks();
	void set_drive(double left_x, double left_y, double right_x, double right_y);
	void set_point_drive(double speed, double direction, double distance, double turnSpeed = 0, double accelScaling = 1, bool blocking = false, double criticalPoint = 400, double criticalMultiplier = 1);
	enum class Module
	{
		Intake = 0,
		Drive = 1,
		Arm = 2,
		Angler =3
	};

	std::map<int, Structure *> module_list;

	void stop();
	void Reset() override;
	std::string name = "Robot";
	// void Debug() override;

	void assign_drive(Mech_Drive* drive_instance);
	void assign_lift(Lift* lift_instance);
	void assign_intake(Structure* intake_instance);

	Mech_Drive* drive = nullptr;
	Lift* lift = nullptr;
	Structure* intake = nullptr;
};

#endif
