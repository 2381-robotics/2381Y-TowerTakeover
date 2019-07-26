#include "main.h"
#include "everything.h"

pros::Controller master (CONTROLLER_MASTER);
pros::Motor leftfront(LF_PORT);
pros::Motor leftback(LB_PORT, true);
pros::Motor rightfront(RF_PORT, true);
pros::Motor rightback(RB_PORT);
pros::Motor intakemotor(INTAKE_PORT);
pros::Motor leftlift(LL_PORT);
pros::Motor rightlift(RL_PORT);
pros::Motor anglermotor(ANGLER_PORT);

int error[7];
int sensor[7];
int setpoint[7];
int integral[7] = {0,0,0,0,0,0,0};
int derivative[7];
int prevError[7] = {0,0,0,0,0,0,0};
int power[7];
int kP [7];
int kI [7];
int kD [7];

void tankdrive() {
	while (true) {
		leftfront.move(master.get_analog(ANALOG_LEFT_Y));
		leftback.move(master.get_analog(ANALOG_LEFT_Y));
		rightfront.move(master.get_analog(ANALOG_RIGHT_Y));
		rightback.move(master.get_analog(ANALOG_RIGHT_Y));
		pros::delay(20);
	}
}

void xdrive() {
	while (true) {
		int right_x = master.get_analog(ANALOG_RIGHT_X);
		int left_y = master.get_analog(ANALOG_LEFT_Y);
		int left_x = master.get_analog(ANALOG_LEFT_X);
		int northwest = left_y+right_x+left_x;
		int northeast = left_y-right_x-left_x;
		int southwest = -1*(left_y+right_x-left_x);
		int southeast = -1*(left_y-right_x+left_x);
		leftfront.move(northwest);
		rightfront.move(northeast);
		leftback.move(southwest);
		rightback.move(southeast);
		pros::delay(20);
	}
}

void lift() {
	while (true) { /*
		/**
		* tune these --> terrence's problem
		* what could help:
		* increase kP until steady, continous oscillations
		* kU = this kP
		* keep kP and see how long each oscillation takes
		* (I think in ms? although idk for sure)
		* pU = this number
		* approximate constant values for a regular PID method
		* (this is by Ziegler-Nichols method):
		* kP = 0.6*kU
		* kI = 1.2*kU/pU
		* kD = 0.075*kU*pU
		* bad idea to write these into code since they're just estimates
		* better to just straight up put in the numbers
		*
		kP[0] = 0;
		kI[0] = 0;
		kD[0] = 0;
	  std::uint32_t now = pros::millis();
		sensor[1] = anglermotor.get_raw_position(&now);
		//sensor is sensor value
		error[LIFTNUMBER] = setpoint[LIFTNUMBER] - sensor[LIFTNUMBER];
		//setpoint is where you want to be,
		//based on our auton (problem for later)
		integral[LIFTNUMBER] += error[LIFTNUMBER];
		if (error[LIFTNUMBER] == 0 || error[LIFTNUMBER] >= setpoint[LIFTNUMBER] || error[LIFTNUMBER] > 9000) {
			//9000 is just meme
			//we need test to figure out
			integral[LIFTNUMBER] = 0;
		}
		derivative[LIFTNUMBER] = error - prevError;
		prevError[LIFTNUMBER] = error[LIFTNUMBER];
		power[LIFTNUMBER] = error[LIFTNUMBER]*kP[LIFTNUMBER] + integral[LIFTNUMBER]*kI[LIFTNUMBER] + derivative[LIFTNUMBER]*kD[LIFTNUMBER];
		*/
		/*if (master.get_digital(DIGITAL_UP) && master.get_digital(DIGITAL_DOWN)) {
			leftlift.move(0);
			rightlift.move(0);
		}*/
		if (master.get_digital(DIGITAL_UP)) {
			//move_velocity?
			leftlift.move(LIFT_POWER/*power[LIFTNUMBER]*/);
			rightlift.move(-1*(LIFT_POWER/*power[LIFTNUMBER]*/));
		}
		else if (master.get_digital(DIGITAL_DOWN)) {
			leftlift.move(-1*(LIFT_POWER/*power[LIFTNUMBER]*/));
			rightlift.move(LIFT_POWER/*power[LIFTNUMBER]*/);
		}
		pros::delay(15);
	}
}

void angler() {
	while (true) {
		if (master.get_digital(DIGITAL_X) && master.get_digital(DIGITAL_Y)) {
			anglermotor.move(0);
		}
		else if (master.get_digital(DIGITAL_X)) {
			anglermotor.move(-1*(ANGLER_POWER));
		}
		else if (master.get_digital(DIGITAL_Y)) {
			anglermotor.move(ANGLER_POWER);
		}
		pros::delay(20);
	}
}

void intake() {
	while (true) {
		if (master.get_digital(DIGITAL_A) && master.get_digital(DIGITAL_B)) {
			intakemotor.move(0);
		}
		else if (master.get_digital(DIGITAL_A)) {
			intakemotor.move(-0.5*(INTAKE_POWER));
		}
		else if (master.get_digital(DIGITAL_B)) {
			intakemotor.move(0.5*(INTAKE_POWER));
		}
		pros::delay(20);
	}
}

void notlift() {
	Ultrasonic ultrasonic = pros::c::ultrasonicInit(PORT_IN, PORT_OUT);
	int error, sensor, setpoint, integral, derivative, prevError = 0, power;
	int kP, kI, kD;
	//set these ^ three
	/**
	* tune these --> terrence's problem
	* what could help:
	* increase kP until steady, continous oscillations
	* kU = this kP
	* keep kP and see how long each oscillation takes
	* (I think in ms? although idk for sure)
	* pU = this number
	* approximate constant values for a regular PID method
	* (this is by Ziegler-Nichols method):
	* kP = 0.6*kU
	* kI = 1.2*kU/pU
	* kD = 0.075*kU*pU
	* bad idea to write these into code since they're just estimates
	* better to just straight up put in the numbers
	*/
	while (true) {
		//sensor is sensor value
		//setpoint is where you want to be,
		//based on our auton (problem for later)
		sensor = pros::c::ultrasonicGet(ultrasonic);
		error = setpoint - sensor;
		integral = integral + error;
		if (error == 0 || error >= setpoint || error > 9000) {
			//9000 is just meme
			//we need test to figure out
			integral = 0;
		}
		derivative = error - prevError;
		prevError = error;
		power = error*kP + integral*kI + derivative*kD;
		intakemotor.move(power);
		pros::delay(15);
	}
}
pros::Motor motor(3);
pros::Motor motor1(8);

void test(){
	while (true) {
		motor.move(master.get_analog(ANALOG_RIGHT_Y));
		motor1.move(-1*(master.get_analog(ANALOG_RIGHT_Y)));
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
	//intake();
	//lift();
	//angler();
	//ultrasonic();
	test();
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
	 rightback.move(0);
}
  pros::delay(100);
}

}
*/
