#include "main.h"
#include "everything.h"
#include "pid.h"
#include "opcontrol.h"
double kp = 0.1;
// double kp = 0.618;
double ki = 0.0;
double kd = 0;
// double kd = 0;
Pid* lift_pid = new Pid(&kp, &ki, &kd);

double driveKp = 1;
double driveKi = 0;
double driveKd = 0;
Pid* left_back_drive_pid = new Pid(&driveKp, &driveKi, &driveKd);
Pid* left_front_drive_pid = new Pid(&driveKp, &driveKi, &driveKd);
Pid* right_back_drive_pid = new Pid(&driveKp, &driveKi, &driveKd);
Pid* right_front_drive_pid = new Pid(&driveKp, &driveKi, &driveKd);


int liftHeight;
int cubeHeight = 360;
int maxLift = 1600;
float liftSpeed = 14.4;


pros::Controller master (CONTROLLER_MASTER);
pros::Motor leftfront(LF_PORT);
pros::Motor leftback(LB_PORT, true);
pros::Motor rightfront(RF_PORT, true);
pros::Motor rightback(RB_PORT);
pros::Motor intakemotor(INTAKE_PORT);
//pros::Motor leftlift(LL_PORT);
//pros::Motor rightlift(RL_PORT);
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
  pros::Motor leftBack (3, false);
  pros::Motor leftFront (2, false);
  pros::Motor rightBack (9, true);
  pros::Motor rightFront (8, true);
void mechDrive(){
	double factor = abs( master.get_analog(ANALOG_RIGHT_X)) / 127.0 ;
    double squareF = factor*factor;
    leftBack.move((master.get_analog(ANALOG_LEFT_Y) - master.get_analog(ANALOG_LEFT_X) + squareF*(master.get_analog(ANALOG_RIGHT_X))));

    leftFront.move((master.get_analog(ANALOG_LEFT_Y) + master.get_analog(ANALOG_LEFT_X) + squareF*(master.get_analog(ANALOG_RIGHT_X))));

    rightBack.move((master.get_analog(ANALOG_LEFT_Y) + master.get_analog(ANALOG_LEFT_X) - squareF*(master.get_analog(ANALOG_RIGHT_X))));

    rightFront.move((master.get_analog(ANALOG_LEFT_Y) - master.get_analog(ANALOG_LEFT_X)  -squareF*(master.get_analog(ANALOG_RIGHT_X))));
}
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


 pros::Motor motor(0); // right
 pros::Motor motor1(0, true); // left
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
// void lift() {
// 	while (true) { /*
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
			//	rightlift.move(master.get_analog(ANALOG_RIGHT_Y));
			//	leftlift.move(-1*(master.get_analog(ANALOG_RIGHT_Y)));
// 				pros::delay(20);
// 		}
//
// }
int intakeSpeed;
pros::Motor intakeLeft(1);
pros::Motor intakeRight(7, true);
void intake(){
	intakeSpeed = 80*(master.get_digital(DIGITAL_L1)-master.get_digital(DIGITAL_R1))+master.get_analog(ANALOG_RIGHT_Y);
	intakeLeft.move(intakeSpeed);
	intakeRight.move(intakeSpeed);

}
//  pros::Motor motor(3);
//  pros::Motor motor2(8);
//  Encoder right = pros::c::encoderInit(3, 4, true);
//  Encoder back = pros::c::encoderInit(1, 2, true);
//  Encoder left = pros::c::encoderInit(6, 5, false);
//
// void test(){
// 	motor.move(master.get_analog(ANALOG_RIGHT_Y));
// 	motor1.move(-1*(master.get_analog(ANALOG_RIGHT_Y)));
// 	pros::lcd::set_text(1, std::to_string(master.get_analog(ANALOG_RIGHT_Y)));
// 		// pros::lcd::set_text(5, "left:" + std::to_string(-1*(motor1.get_position())));
// 		// pros::lcd::set_text(6, "right:" + std::to_string(motor.get_position()));
// 		double liftAverage =(-1*motor1.get_position() + motor.get_position())/2;
// 		pros::lcd::set_text(2, "average:" + (std::to_string(liftAverage)));
// 		awning_output = awning_pid->Update(liftHeight, liftAverage);
// 		motor.move_velocity(awning_output);
// 		motor1.move_velocity(-awning_output);
// }


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
//
// void intake() {
// 	while (true) {
// 		if (master.get_digital(DIGITAL_A) && master.get_digital(DIGITAL_B)) {
// 			intakemotor.move(0);
// 		}
// 		else if (master.get_digital(DIGITAL_A)) {
// 			intakemotor.move(-0.5*(INTAKE_POWER));
// 		}
// 		else if (master.get_digital(DIGITAL_B)) {
// 			intakemotor.move(0.5*(INTAKE_POWER));
// 		}
// 		pros::delay(20);
// 	}
// }

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
 double drivePid(pros::Motor motor, Pid* pid, double target) {
   double result = pid->Update(target, motor.get_actual_velocity());
   return result;
 }

double leftFrontSetpoint, leftBackSetpoint, rightFrontSetpoint, rightBackSetpoint;
double leftFrontMotorValue, leftBackMotorValue, rightFrontMotorValue, rightBackMotorValue;
 void opcontrol() {
   pros::Motor motor1 (8);
   pros::Motor leftBack (2, false);
   pros::Motor leftFront (3, false);
   pros::Motor rightBack (9, true);
   pros::Motor rightFront (8, true);




   while (true) {
     double factor = abs( master.get_analog(ANALOG_RIGHT_X)) / 127.0 ;
     double squareF = factor*factor;
     leftBackSetpoint = (master.get_analog(ANALOG_LEFT_Y) - master.get_analog(ANALOG_LEFT_X) + squareF*(master.get_analog(ANALOG_RIGHT_X)));
     leftFrontSetpoint = (master.get_analog(ANALOG_LEFT_Y) + master.get_analog(ANALOG_LEFT_X) + squareF*(master.get_analog(ANALOG_RIGHT_X)));
     rightBackSetpoint = (master.get_analog(ANALOG_LEFT_Y) + master.get_analog(ANALOG_LEFT_X) - squareF*(master.get_analog(ANALOG_RIGHT_X)));
     rightFrontSetpoint = (master.get_analog(ANALOG_LEFT_Y) - master.get_analog(ANALOG_LEFT_X)  -squareF*(master.get_analog(ANALOG_RIGHT_X)));
     leftBackMotorValue = drivePid(leftBack, left_back_drive_pid, leftBackSetpoint);
		 leftFrontMotorValue = drivePid(leftFront, left_front_drive_pid, leftFrontSetpoint);
		 rightBackMotorValue = drivePid(rightBack, right_back_drive_pid, rightBackSetpoint);
     rightFrontMotorValue = drivePid(rightFront, right_front_drive_pid, rightFrontSetpoint);


     leftBack.move(leftBackMotorValue);
     pros::lcd::set_text(0, "leftBack:" + std::to_string((leftBack.get_position())));

     leftFront.move(leftFrontMotorValue);
     pros::lcd::set_text(2, "leftFront:" + std::to_string((leftFront.get_position())));

     rightBack.move(rightBackMotorValue);
     pros::lcd::set_text(1, "rightBack:" + std::to_string((rightBack.get_position())));

     rightFront.move(rightFrontMotorValue);
     pros::lcd::set_text(3, "rightFront:" + std::to_string((rightFront.get_position())));


     pros::lcd::set_text(4, "motor value leftBack:" + std::to_string(leftBackMotorValue));

     pros::lcd::set_text(5, "motor value leftFront:" + std::to_string(leftFrontMotorValue));

     pros::lcd::set_text(6, "motor value rightBack:" + std::to_string(rightBackMotorValue));

     pros::lcd::set_text(7, "motor value rightFront:" + std::to_string(rightFrontMotorValue));

     pros::delay(20);
   }
 }

//
// vision::signature SIG_1 (1, 0, 0, 0, 0, 0, 0, 3.000, 0);
// vision::signature SIG_2 (2, 0, 0, 0, 0, 0, 0, 3.000, 0);
// vision::signature TOPBLUE (3, 0, 0, 0, 0, 0, 0, 2.900, 0);
// vision::signature SIG_4 (4, -2979, 1, -1489, 697, 7877, 4287, 0.000, 0);
// vision::signature SIG_5 (5, -2919, 1, -1459, 841, 8643, 4742, 1.200, 0);
// vision::signature SIG_6 (6, 0, 0, 0, 0, 0, 0, 3.000, 0);
// vision::signature SIG_7 (7, 0, 0, 0, 0, 0, 0, 3.000, 0);
// vex::vision vision1 ( vex::PORT1, 50, SIG_1, SIG_2, TOPBLUE, SIG_4, SIG_5, SIG_6, SIG_7 );
//
// vision::signature SIG_1 (1, -4275, 1, -2137, -8121, -983, -4552, 1.100, 0);
// vision::signature SIG_2 (2, 0, 0, 0, 0, 0, 0, 3.000, 0);
// vision::signature SIG_3 (3, 0, 0, 0, 0, 0, 0, 3.000, 0);
// vision::signature SIG_4 (4, 0, 0, 0, 0, 0, 0, 3.000, 0);
// vision::signature SIG_5 (5, 0, 0, 0, 0, 0, 0, 3.000, 0);
// vision::signature SIG_6 (6, 0, 0, 0, 0, 0, 0, 3.000, 0);
// vision::signature SIG_7 (7, 0, 0, 0, 0, 0, 0, 3.000, 0);
// vex::vision vision1 ( vex::PORT1, 50, SIG_1, SIG_2, SIG_3, SIG_4, SIG_5, SIG_6, SIG_7 );
// *
// void sensors() {
//   	pros::Vision sensor (VISION_PORT);
// 	  sensor.set_zero_point(pros::E_VISION_ZERO_CENTER);
// 	  // values acquired from the vision utility
// 	  pros::vision_signature_s_t RED_SIG =
// 	    pros::Vision::signature_from_utility(EXAMPLE_SIG, -6613, -4417, -5516, -3847, -2473, -3160, 0.700, 0);
// 	  sensor.set_signature(EXAMPLE_SIG, &RED_SIG);
// 	  while (true) {
//     	pros::vision_object_s_t rtn = sensor.get_by_sig(0, EXAMPLE_SIG);
//     // Gets the largest object of the EXAMPLE_SIG signature
//     //pros::lcd::set_text(2, std::to_string(rtn.signature));
// 		pros::lcd::set_text(1, std::to_string(sensor.get_object_count()));
//     //pros::lcd::set_text(3, std::to_string(rtn.angle));
//     pros::lcd::set_text(4, std::to_string(rtn.x_middle_coord));
//     pros::lcd::set_text(5, std::to_string(rtn.y_middle_coord));
//     pros::lcd::set_text(6, std::to_string(rtn.width));
//     pros::lcd::set_text(7, std::to_string(rtn.height));
//     if (master.get_digital(DIGITAL_B)) {
// 		    if((rtn.width>50||rtn.height>50)&&(abs(rtn.x_middle_coord)<70)&&(rtn.width<310)){
// 					leftfront.move(-50);
// 					leftback.move(50);
// 					rightfront.move(50);
// 					rightback.move(-50);
//         }else{
// 					leftfront.move(0);
// 					leftback.move(0);
// 					rightfront.move(0);
// 					rightback.move(0);
//
//     }
// 	}
// 	else if(master.get_digital(DIGITAL_A)){
//       if((rtn.width>50||rtn.height>50)&&(rtn.signature==1)&&(rtn.x_middle_coord<-30)){
//         pros::lcd::set_text(3, "greater Than 0");
// 				leftfront.move(50);
// 				leftback.move(-50);
// 				rightfront.move(-50);
// 				rightback.move(50);
//       } else if((rtn.width>50||rtn.height>50)&&(rtn.signature==1)&&(rtn.x_middle_coord>30)) {
//           pros::lcd::set_text(3, "less than 0");
// 					leftfront.move(-50);
// 					leftback.move(50);
// 					rightfront.move(50);
// 					rightback.move(-50);
//         }
// 				else if ((rtn.width>50||rtn.height>50)&&(rtn.signature==1)) {
// 					pros::lcd::set_text(3, "terrence");
// 				}
// 				else{
//           pros::lcd::set_text(3, "nothing");
// 					leftfront.move(0);
// 					leftback.move(0);
// 					rightfront.move(0);
// 					rightback.move(0);
//
//     // Prints "sig: 1"
//
//   }}else{
//    pros::lcd::set_text(3, "terrence2");
// 	 leftfront.move(0);
// 	 leftback.move(0);
// 	 rightfront.move(0);
// 	 rightback.move(0);
// }
//   pros::delay(100);
// }
//
// }
