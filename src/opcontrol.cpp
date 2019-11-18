#include "opcontrol.h"

#include "main.h"
#include "ports.h"

#include "utils/pid.h"
#include "utils/motor_controller.hpp"
#include "utils/robot/drive/mech_drive.hpp"

#include <list>
#include <map>
#include "globals.hpp"
using namespace pros;
Motor testMotorLeft (2, true);
Motor testMotorRight (3, false);
std::array<double, 3> pidValues = {0, 0.5, 0};
Motor_Controller* leftMotorController = new Motor_Controller(&pidValues[0], &pidValues[1], &pidValues[2], &testMotorLeft);
Motor_Controller *rightMotorController = new Motor_Controller(&pidValues[0], &pidValues[1], &pidValues[2], &testMotorRight);

void opcontrol()
{
  while (true)
  {
    
    //  testMotorLeft.move(50);
     leftMotorController->Set_Speed((master.get_digital(DIGITAL_L1) - master.get_digital(DIGITAL_R1)) * -60 + (master.get_digital(DIGITAL_L2) - master.get_digital(DIGITAL_R2)) * -30);
     rightMotorController->Set_Speed((master.get_digital(DIGITAL_L1) - master.get_digital(DIGITAL_R1)) * -60 + (master.get_digital(DIGITAL_L2) - master.get_digital(DIGITAL_R2)) * -30);
    //  robot->set_drive(master.get_analog(ANALOG_LEFT_X), master.get_analog(ANALOG_LEFT_Y), master.get_analog(ANALOG_RIGHT_X), master.get_analog(ANALOG_RIGHT_Y));
    //  robot->lift->Increment_Height(master.get_digital_new_press(DIGITAL_X) - master.get_digital_new_press(DIGITAL_B));
    //  robot->lift->Smooth_Lift(master.get_digital(DIGITAL_UP) - master.get_digital(DIGITAL_DOWN));
    // //  pros::lcd::set_text(3, "right_front_motor:" + std::to_string(robot->lift->Get_Target()));
    //   robot->lift->Move_Lift();
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
