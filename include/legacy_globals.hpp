#ifndef __LEGACY_GLOBALS__H__
#define __LEGACY_GLOBALS__H__

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

// Lift
extern std::array<double,3> lift_pid_values, master_lift_pid_values;
extern pros::Motor left_lift_motor, right_lift_motor;
extern double cube_height, lift_max_height, lift_min_height, lift_speed;

// Ultras 
extern pros::ADIUltrasonic ultra_left;
extern pros::ADIUltrasonic ultra_right;
extern UltraFinder* ultra_finder;

// Vision
extern pros::Vision vision_sensor;
extern VisionIndexer* vision_indexer;

//Angler
extern std::array<double,3> angler_pid_values;
extern pros::Motor angler_motor;
extern double angler_min_height, angler_max_height, angler_speed;
extern Angler* angler;
//Arm
extern std::array<double,3> arm_pid_values;
extern pros::Motor arm_motor;
extern double arm_min_height, arm_max_height, arm_speed;
extern Arm* arm;
extern int arm_mode;




#endif  //!__LEGACY_GLOBALS__H__