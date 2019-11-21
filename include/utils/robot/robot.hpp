/**
 * \file robot.hpp
 * \brief
 */
#ifndef ROBOT_H_
#define ROBOT_H_
#include "utils/robot/drive/drive.hpp"
#include "utils/robot/structure.hpp"
#include "utils/robot/lift/lift.hpp"
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

	void begin_tasks();
	void set_drive(double left_x, double left_y, double right_x, double right_y);
	void set_point_drive(double speed, double direction, double distance);

	void stop();
	void create() override;

	void assign_drive(Drive* drive_instance);
	void assign_lift(Lift* lift_instance);
	void assign_intake(Structure* intake_instance);


	Drive* drive = nullptr;
	Lift* lift = nullptr;
	Structure* intake = nullptr;
};

#endif
