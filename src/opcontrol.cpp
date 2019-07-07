#include "main.h"
#include "everything.h"

pros::Controller master (CONTROLLER_MASTER);
pros::Motor leftfront(LF_PORT);
pros::Motor leftback(LB_PORT, true);
pros::Motor rightfront(RF_PORT, true);
pros::Motor rightback(RB_PORT);
pros::Motor intakemotor(INTAKE_PORT);

void tankdrive() {
	while (true) {
		leftfront.move(master.get_analog(ANALOG_LEFT_Y));
		leftback.move(master.get_analog(ANALOG_LEFT_Y));
		rightfront.move(master.get_analog(ANALOG_RIGHT_Y));
		rightback.move(master.get_analog(ANALOG_RIGHT_Y));
		pros::delay(20);
	}
}

void xdrive(){
	while (true) {
		int right_x = master.get_analog(ANALOG_RIGHT_X);
		int left_y = master.get_analog(ANALOG_LEFT_Y);
		int left_x = master.get_analog(ANALOG_LEFT_X);
		leftfront.move(left_y+right_x+left_x);
		rightfront.move(left_y-right_x-left_x);
		leftback.move(-1*(left_y+right_x-left_x));
		rightback.move(-1*(left_y-right_x+left_x));
		pros::delay(20);
	}
}

/**
vision::signature SIG_1 (1, 0, 0, 0, 0, 0, 0, 3.000, 0);
vision::signature SIG_2 (2, 0, 0, 0, 0, 0, 0, 3.000, 0);
vision::signature TOPBLUE (3, 0, 0, 0, 0, 0, 0, 2.900, 0);
vision::signature SIG_4 (4, -2979, 1, -1489, 697, 7877, 4287, 0.000, 0);
vision::signature SIG_5 (5, -2919, 1, -1459, 841, 8643, 4742, 1.200, 0);
vision::signature SIG_6 (6, 0, 0, 0, 0, 0, 0, 3.000, 0);
vision::signature SIG_7 (7, 0, 0, 0, 0, 0, 0, 3.000, 0);
vex::vision vision1 ( vex::PORT1, 50, SIG_1, SIG_2, TOPBLUE, SIG_4, SIG_5, SIG_6, SIG_7 );

vision::signature SIG_1 (1, -4275, 1, -2137, -8121, -983, -4552, 1.100, 0);
vision::signature SIG_2 (2, 0, 0, 0, 0, 0, 0, 3.000, 0);
vision::signature SIG_3 (3, 0, 0, 0, 0, 0, 0, 3.000, 0);
vision::signature SIG_4 (4, 0, 0, 0, 0, 0, 0, 3.000, 0);
vision::signature SIG_5 (5, 0, 0, 0, 0, 0, 0, 3.000, 0);
vision::signature SIG_6 (6, 0, 0, 0, 0, 0, 0, 3.000, 0);
vision::signature SIG_7 (7, 0, 0, 0, 0, 0, 0, 3.000, 0);
vex::vision vision1 ( vex::PORT1, 50, SIG_1, SIG_2, SIG_3, SIG_4, SIG_5, SIG_6, SIG_7 );
*
void sensors() {
  	pros::Vision sensor (VISION_PORT);
	  sensor.set_zero_point(pros::E_VISION_ZERO_CENTER);
	  // values acquired from the vision utility
	  pros::vision_signature_s_t RED_SIG =
	    pros::Vision::signature_from_utility(EXAMPLE_SIG, -6613, -4417, -5516, -3847, -2473, -3160, 0.700, 0);
	  sensor.set_signature(EXAMPLE_SIG, &RED_SIG);
	  while (true) {
    	pros::vision_object_s_t rtn = sensor.get_by_sig(0, EXAMPLE_SIG);
    // Gets the largest object of the EXAMPLE_SIG signature
    //pros::lcd::set_text(2, std::to_string(rtn.signature));
		pros::lcd::set_text(1, std::to_string(sensor.get_object_count()));
    //pros::lcd::set_text(3, std::to_string(rtn.angle));
    pros::lcd::set_text(4, std::to_string(rtn.x_middle_coord));
    pros::lcd::set_text(5, std::to_string(rtn.y_middle_coord));
    pros::lcd::set_text(6, std::to_string(rtn.width));
    pros::lcd::set_text(7, std::to_string(rtn.height));
    if (master.get_digital(DIGITAL_B)) {
		    if((rtn.width>50||rtn.height>50)&&(abs(rtn.x_middle_coord)<70)&&(rtn.width<310)){
					leftfront.move(-50);
					leftback.move(50);
					rightfront.move(50);
					rightback.move(-50);
        }else{
					leftfront.move(0);
					leftback.move(0);
					rightfront.move(0);
					rightback.move(0);

    }
	}
	else if(master.get_digital(DIGITAL_A)){
      if((rtn.width>50||rtn.height>50)&&(rtn.signature==1)&&(rtn.x_middle_coord<-30)){
        pros::lcd::set_text(3, "greater Than 0");
				leftfront.move(50);
				leftback.move(-50);
				rightfront.move(-50);
				rightback.move(50);
      } else if((rtn.width>50||rtn.height>50)&&(rtn.signature==1)&&(rtn.x_middle_coord>30)) {
          pros::lcd::set_text(3, "less than 0");
					leftfront.move(-50);
					leftback.move(50);
					rightfront.move(50);
					rightback.move(-50);
        }
				else if ((rtn.width>50||rtn.height>50)&&(rtn.signature==1)) {
					pros::lcd::set_text(3, "terrence");
				}
				else{
          pros::lcd::set_text(3, "nothing");
					leftfront.move(0);
					leftback.move(0);
					rightfront.move(0);
					rightback.move(0);

    // Prints "sig: 1"

  }}else{
   pros::lcd::set_text(3, "terrence2");
	 leftfront.move(0);
	 leftback.move(0);
	 rightfront.move(0);
	 rightback.move(-0);
}
  pros::delay(100);
}

}
*/
void ultrasonic(){
	Ultrasonic ultrasonic = pros::c::ultrasonicInit(PORT_IN, PORT_OUT);
}

void intake() {
	while (true) {
		intakemotor.move(-1*(master.get_analog(ANALOG_RIGHT_Y)));
		pros::delay(20);
	}
}

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
	//xdrive();
	intake();
	//ultrasonic();
}
