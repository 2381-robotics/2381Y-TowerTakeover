#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "robot/robot.hpp"
#include <array>
#include "api.h"
#include "ports.h"
#include "okapi/api.hpp"
#include "autonomous/auton_control.hpp"
#include "robot/modules/angler.hpp"
#include "robot/modules/arm.hpp"
#include "robot/modules/intake.hpp"
#include "master_controller.hpp"
#include "autonomous/auto_sequence.hpp"
#include "robot/modules/drive/position_tracker.hpp"
#include "robot/sensors/vision_indexer.hpp"
#include "robot/sensors/ultra_align.hpp"
#include "robot/modules/shooter.hpp"
#include "robot/modules/indexer.hpp"

#define DELAY_INTERVAL 20


extern bool STOP;
extern Robot* robot;
extern AutonControl* auton_control;
extern pros::Controller master;
extern MasterController* master_control;
extern std::uint32_t now;
// Autonomous


extern double autonomous_increment;

extern void driver();

extern bool Competition_Env;

// Drive
extern std::array<double,3> left_back_pid_values, right_back_pid_values, left_front_pid_values, right_front_pid_values;
extern std::array<double, 3> master_drive_pid_values; //ku = 1.10

extern pros::Motor left_front_motor, left_back_motor, right_front_motor, right_back_motor;

//intake
extern std::array<double, 3> pid_intake_left_values, pid_intake_right_values;
extern std::array<double, 3> master_intake_pid_values;
extern pros::Motor intakeMotorLeft, intakeMotorRight;

extern Intake* intake;


// Shooter
extern std::array<double,3> pid_shooter_values;
extern pros::Motor shootMotor;
extern Shooter* shooter;

// Indexer
extern std::array<double,3> pid_indexer_values;
extern pros::ADIAnalogIn index_sensor_top;
extern pros::Motor indexMotor;
extern Indexer* indexer;

// Tracking Wheels
extern Position_Tracker* position_tracker;
extern std::array<int,3> h_enc_ports, v_enc_ports;
extern std::array<double,3> wheel_diameters, wheel_offsets;
extern pros::ADIEncoder h_enc, v_enc;
extern pros::Imu imu;



#endif
