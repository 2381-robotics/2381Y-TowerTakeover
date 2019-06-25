#include "main.h"
#include "api.h"
/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	pros::Controller master(CONTROLLER_MASTER);
	pros::Motor leftfront(1);
	pros::Motor leftback(2);
	pros::Motor rightfront(3);
	pros::Motor rightback(4);
	while (true) {
		leftfront.move(master.get_analog(ANALOG_LEFT_Y));
		leftback.move(master.get_analog(ANALOG_LEFT_Y));
		rightfront.move(master.get_analog(ANALOG_RIGHT_Y));
		rightback.move(master.get_analog(ANALOG_RIGHT_Y));
		pros::delay(20);
	}
}
