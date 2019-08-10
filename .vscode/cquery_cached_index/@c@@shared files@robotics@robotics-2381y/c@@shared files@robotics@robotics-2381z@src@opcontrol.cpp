#include "main.h"
#include "everything.h"
#include "pid.h"
#include "opcontrol.h"

double kp[3] = {0.618, 0.1, 1};
double ki[3] = {0, 0, 0};
double kd[3] = {1.454, 0, 0};

auto lift_pid = new Pid(&kp[0], &ki[0], &kd[0]);
auto left_intake_pid = new Pid(&kp[1], &ki[1], &kd[1]);
auto right_intake_pid = new Pid(&kp[2], &ki[2], &kd[2]);

int liftHeight;
int cubeHeight = 178;
int maxLift = 1600;
float liftSpeed = 14.4;

float leftIntakeSpeed = 10, rightIntakeSpeed = 10;
int leftIntakeHeight, rightIntakeHeight;
int maxHeight;
//set this later


pros::Controller master (CONTROLLER_MASTER);
pros::Motor leftfront(LF_PORT);
pros::Motor leftback(LB_PORT, true);
pros::Motor rightfront(RF_PORT, true);
pros::Motor rightback(RB_PORT);
pros::Motor leftintake(LI_PORT, true);
pros::Motor rightintake(RI_PORT);

// pros::Motor leftlift(LL_PORT);
// pros::Motor rightlift(RL_PORT);

void tankdrive() {
		leftfront.move(master.get_analog(ANALOG_LEFT_Y));
		leftback.move(master.get_analog(ANALOG_LEFT_Y));
		rightfront.move(master.get_analog(ANALOG_RIGHT_Y));
		rightback.move(master.get_analog(ANALOG_RIGHT_Y));
		pros::delay(20);
}

void xdrive() {

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

}

void xDrivePID() {
	// pros::lcd::set_text(3, "leftFront:" + std::to_string(leftfront.get_position()));
	// pros::lcd::set_text(4, "rightFront:" + std::to_string(rightfront.get_position()));
	// pros::lcd::set_text(5, "leftBack:" + std::to_string(leftback.get_position()));
	// pros::lcd::set_text(6, "rightBack:" + std::to_string(rightback.get_position()));
		// int right_x = master.get_analog(ANALOG_RIGHT_X);
		// int left_y = master.get_analog(ANALOG_LEFT_Y);
		// int left_x = master.get_analog(ANALOG_LEFT_X);
		// int northwest = left_y+right_x+left_x;
		// int northeast = left_y-right_x-left_x;
		// int southwest = -1*(left_y+right_x-left_x);
		// int southeast = -1*(left_y-right_x+left_x);
		// leftfront.move(northwest);
		// rightfront.move(northeast);
		// leftback.move(southwest);
		// rightback.move(southeast);

}

// void lift() {
// 	while (true) { /*
// 		/**
// 		* tune these --> terrence's problem
// 		* what could help:
// 		* increase kP until steady, continous oscillations
// 		* kU = this kP
// 		* keep kP and see how long each oscillation takes
// 		* (I think in ms? although idk for sure)
// 		* pU = this number
// 		* approximate constant values for a regular PID method
// 		* (this is by Ziegler-Nichols method):
// 		* kP = 0.6*kU
// 		* kI = 1.2*kU/pU
// 		* kD = 0.075*kU*pU
// 		* bad idea to write these into code since they're just estimates
// 		* better to just straight up put in the numbers
// 		*
// 		kP[0] = 0;
// 		kI[0] = 0;
// 		kD[0] = 0;
// 	  std::uint32_t now = pros::millis();
// 		sensor[1] = anglermotor.get_raw_position(&now);
// 		//sensor is sensor value
// 		error[LIFTNUMBER] = setpoint[LIFTNUMBER] - sensor[LIFTNUMBER];
// 		//setpoint is where you want to be,
// 		//based on our auton (problem for later)
// 		integral[LIFTNUMBER] += error[LIFTNUMBER];
// 		if (error[LIFTNUMBER] == 0 || error[LIFTNUMBER] >= setpoint[LIFTNUMBER] || error[LIFTNUMBER] > 9000) {
// 			//9000 is just meme
// 			//we need test to figure out
// 			integral[LIFTNUMBER] = 0;
// 		}
// 		derivative[LIFTNUMBER] = error - prevError;
// 		prevError[LIFTNUMBER] = error[LIFTNUMBER];
// 		power[LIFTNUMBER] = error[LIFTNUMBER]*kP[LIFTNUMBER] + integral[LIFTNUMBER]*kI[LIFTNUMBER] + derivative[LIFTNUMBER]*kD[LIFTNUMBER];
// 		*/
// 		/*if (master.get_digital(DIGITAL_UP) && master.get_digital(DIGITAL_DOWN)) {
// 			leftlift.move(0);
// 			rightlift.move(0);
// 		}*/
// 			//	rightlift.move(master.get_analog(ANALOG_RIGHT_Y));
// 			//	leftlift.move(-1*(master.get_analog(ANALOG_RIGHT_Y)));
// 				pros::delay(20);
// 		}
//
// }
 pros::Motor motor(RL_PORT); // right
 pros::Motor motor1(LL_PORT, true); // left
 Encoder right = pros::c::encoderInit(3, 4, true);
 Encoder back = pros::c::encoderInit(1, 2, true);
 Encoder left = pros::c::encoderInit(6, 5, false);

 void pidLift() {
 	if (master.get_digital_new_press(DIGITAL_X)) {
       liftHeight = (liftHeight + cubeHeight > maxLift) ? maxLift : (liftHeight+cubeHeight);
     }
 	if (master.get_digital_new_press(DIGITAL_B)) {
 		liftHeight = (liftHeight - cubeHeight < 0) ? 0 : (liftHeight-cubeHeight);
 	}
 }

 void autoLift() {
 	if(master.get_digital(DIGITAL_UP)){
 		liftHeight = (liftHeight + liftSpeed > maxLift) ? maxLift : (liftHeight+liftSpeed);
 	}
 	if(master.get_digital(DIGITAL_DOWN)){
 		liftHeight = (liftHeight - liftSpeed < 0) ? 0 : (liftHeight-liftSpeed);
 	}
 }
double liftAverage;
void lift() {
	pros::lcd::set_text(3, std::to_string(master.get_analog(ANALOG_RIGHT_Y)));
		pros::lcd::set_text(5, "left:" + std::to_string((motor1.get_position())));
		pros::lcd::set_text(6, "right:" + std::to_string(motor.get_position()));
		liftAverage =(motor1.get_position() + motor.get_position())/2;
		pros::lcd::set_text(2, "average:" + (std::to_string(liftAverage)));
		lift_output = lift_pid->Update(liftHeight, liftAverage);
		pros::lcd::set_text(4, "liftoutput:" + (std::to_string(lift_output)));
		pros::lcd::set_text(7, "liftheight:" + (std::to_string(liftHeight)));
		// liftHeight+=master.get_analog(ANALOG_RIGHT_Y);
		pidLift();
		autoLift();
		motor.move_velocity(lift_output);
		motor1.move_velocity(lift_output);

}

void liftPIDTest() {
	liftAverage =(motor1.get_position() + motor.get_position())/2;
	// pros::lcd::set_text(2, "average:" + (std::to_string(liftAverage)));
	lift_output = lift_pid->Update(600, liftAverage);
	// pros::lcd::set_text(4, "liftoutput:" + (std::to_string(lift_output)));
	motor.move_velocity(lift_output);
	motor1.move_velocity(-lift_output);

}
void autoIntake() {
	//for slowdown
	int leftCheck = 0, rightCheck = 0;
	//just to be safe...
	if (leftIntakeHeight > maxHeight) {
		leftIntakeHeight = maxHeight - leftIntakeSpeed;
	}
	if (rightIntakeHeight > maxHeight) {
		rightIntakeHeight = maxHeight - rightIntakeSpeed;
	}
	if (leftIntakeHeight < -maxHeight) {
		leftIntakeHeight = -maxHeight + leftIntakeSpeed;
	}
	if (rightIntakeHeight < -maxHeight) {
		rightIntakeHeight = -maxHeight + rightIntakeSpeed;
	}
	//main chunk of code
 	if(master.get_digital(DIGITAL_L1)){
	 //to accelerate change in intake vs outtake
	 if (leftIntakeHeight < 0){
		 leftIntakeHeight = 0;
	 }
	 //acceleration; maybe accelerate faster later on
	 leftIntakeHeight += leftIntakeSpeed;
	 //for slowdown
	 leftCheck = 1;
 }
 if(master.get_digital(DIGITAL_L2)){
	 if (leftIntakeHeight > 0){
		 leftIntakeHeight = 0;
	 }
	 leftIntakeHeight -= leftIntakeSpeed;
	 leftCheck = 1;
 }
 if(master.get_digital(DIGITAL_R1)){
	 if (rightIntakeHeight < 0){
		 rightIntakeHeight = 0;
	 }
	 rightIntakeHeight += rightIntakeSpeed;
	 rightCheck = 1;
	}
 if(master.get_digital(DIGITAL_R2)){
	 if (rightIntakeHeight > 0){
		 rightIntakeHeight = 0;
	 }
	 rightIntakeHeight -= rightIntakeSpeed;
	 rightCheck = 1;
 }
	//slowdown
	if (leftCheck == 0){
		if (leftIntakeHeight > 5*leftIntakeSpeed) {
			leftIntakeHeight -= 3*leftIntakeSpeed;
		}
		else if (leftIntakeHeight < -5*leftIntakeSpeed) {
			leftIntakeHeight += 3*leftIntakeSpeed;
		}
		else {
			leftIntakeHeight = 0;
		}
	}
	if (rightCheck == 0){
		if (rightIntakeHeight > 5*rightIntakeSpeed) {
			rightIntakeHeight -= 3*rightIntakeSpeed;
		}
		else if (rightIntakeHeight < -5*rightIntakeSpeed) {
			rightIntakeHeight += 3*rightIntakeSpeed;
		}
		else {
			rightIntakeHeight = 0;
		}
	}
	leftCheck = 0;
	rightCheck = 0;
}

void intake() {
	// if (master.get_digital(DIGITAL_R1)){
	// 	rightintake.move_velocity(100);
	// 	leftintake.move_velocity(-100);
	// }
	// if (master.get_digital(DIGITAL_R2)){
	// 	rightintake.move_velocity(-100);
	// 	leftintake.move_velocity(100);
	// }
	left_intake_output = left_intake_pid->Update(leftIntakeSpeed, leftintake.get_position());
	right_intake_output = right_intake_pid->Update(rightIntakeSpeed, rightintake.get_position());
	rightintake.move_velocity(right_intake_output);
	leftintake.move_velocity(-left_intake_output);
	autoIntake();
}
int intakeRightSpeed, intakeLeftSpeed;
void intakeLeft() {
	intakeLeftSpeed = 100*(master.get_digital(DIGITAL_L1) - master.get_digital(DIGITAL_L2))+master.get_analog(ANALOG_LEFT_Y);
	leftintake.move_velocity(intakeLeftSpeed);
	// pros::lcd::set_text(2, "intake left speed:" + (std::to_string(intakeLeftSpeed)));

}
void intakeRight() {
		intakeRightSpeed = 100*(master.get_digital(DIGITAL_R1) - master.get_digital(DIGITAL_R2))+master.get_analog(ANALOG_LEFT_Y);
		rightintake.move_velocity(intakeRightSpeed);
		// pros::lcd::set_text(3, "intake right speed:" + (std::to_string(intakeRightSpeed)));

}
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

void test() {
	if (master.get_digital(DIGITAL_UP)) {
		leftfront.move(-100);
		rightback.move(100);
	}
	if (master.get_digital(DIGITAL_DOWN)) {
		leftfront.move(100);
		rightback.move(-100);
	}
	if (master.get_digital(DIGITAL_LEFT)){
		leftback.move(-100);
		rightfront.move(100);
	}
	if (master.get_digital(DIGITAL_RIGHT)){
		leftback.move(100);
		rightfront.move(-100);
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
	while(true){
		// xdrive();
		// xDrivePID();
		// autoIntake();
		intakeLeft();
		intakeRight();
		//ultrasonic();
		lift();
		test();
		pros::lcd::set_text(1, "hello");
		// pros::lcd::set_text(2, "leftfront:" + std::to_string(leftfront.get_position()));
		// pros::lcd::set_text(3, "rightfront:" + std::to_string(rightfront.get_position()));
		// pros::lcd::set_text(4, "leftback:" + std::to_string(leftback.get_position()));
		// pros::lcd::set_text(5, "rightback:" + std::to_string(rightback.get_position()));
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
	 rightback.move(0);
}
  pros::delay(100);
}
}
*/
