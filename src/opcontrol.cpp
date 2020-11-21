#include "opcontrol.h"

#include "main.h"
#include "ports.h"
#include "robot/modules/intake.hpp"

#include "robot/control/pid.hpp"
#include "robot/control/motor_controller.hpp"
#include "robot/modules/drive/mech_drive.hpp"
#include "robot/modules/angler.hpp"
#include "master_controller.hpp" 
#include <functional>
  
#include <list>  
#include <map> 
#include <string>
#include "globals.hpp"

using namespace pros;

// Motor FlywheelMotor (15, false);
// Motor IntakeMotor (15,false);
// Motor IntakeMotor2 (16,true);

// Motor InternalIntakeMotor (13, false);
// Motor FlywheelMotor(14, false);

array<double, 3> yeet = {0.3,0,0};      
// Motor_Controller* FlywheelController = new Motor_Controller( &yeet[0], &yeet[1], &yeet[2], &FlywheelMotor);
// Motor_Controller* IntakeController = new Motor_Controller( &yeet[0], &yeet[1], &yeet[2], &IntakeMotor);

bool spinFly = false;
//   Intake * intake = new Intake();
int anglerGoingDown = 0;
bool snapshot = false;

void driver()  {
    if(master.get_digital_new_press(DIGITAL_L2));
    {
      spinFly = !spinFly; 
    }
    // FlywheelController->Set_Speed(spinFly *300);
    // IntakeController->Set_Speed(master.get_digital(DIGITAL_L1) * 300);
  // FlywheelMotor.move_velocity(200* spinFly);
  
  // IntakeMotor.move(127 *(master.get_digital(DIGITAL_L1) -master.get_digital(DIGITAL_L2)));
  // IntakeMotor2.move( 127 * (master.get_digital(DIGITAL_L1) -master.get_digital(DIGITAL_L2)));
  // InternalIntakeMotor.move(100 * (master.get_digital(DIGITAL_R1) - master.get_digital(DIGITAL_R2)));
  // FlywheelMotor.move(127 * (master.get_digital(DIGITAL_A) - master.get_digital(DIGITAL_B)));
  lcd::set_text(4, "LINE TRACK:" + to_string(index_sensor_top.get_value()));
    // angler->Multiply_Max_Speed(2);
  intake->Set_Intake((master.get_digital(DIGITAL_L1) * 127 - master.get_digital(DIGITAL_L2) * 50 - 110* (master.get_digital(DIGITAL_L2) && master.get_digital(DIGITAL_L1))));
  shootMotor.move(127*(master.get_digital(DIGITAL_R1) - master.get_digital(DIGITAL_R2)));
  // intake->Set_Intake((master.get_digital(DIGITAL_L1) * 127 - master.get_digital(DIGITAL_L2) * 50 - 110* (master.get_digital(DIGITAL_L2) && master.get_digital(DIGITAL_L1)) - 30* master.get_digital(DIGITAL_RIGHT)* vision_indexer->Check_Object() ));
  robot->set_drive(master.get_analog(ANALOG_LEFT_X), master.get_analog(ANALOG_LEFT_Y), master.get_analog(ANALOG_RIGHT_X), master.get_analog(ANALOG_RIGHT_Y));
  /*done just upload*/

  // intakeMotorLeft.move_voltage(master.get_digital(DIGITAL_L1)* 12000 - master.get_digital(DIGITAL_L2) * 6000);
  // intakeMotorRight.move_voltage(master.get_digital(DIGITAL_L1) * 12000 - master.get_digital(DIGITAL_L2) * 6000);
  // vision_object_s_t object_arr[4];
  // vision_sensor.read_by_sig(0, 2, 4, object_arr);
  // lcd::set_text(2, to_string(object_arr[0].width) + "width" + to_string(vision_indexer->Get_Object().width) + " " + to_string(vision_indexer->vision_sensor->get_by_sig(0, 2).width));
    
    
    // pros::lcd::set_text(4, "arm mode" + to_string(arm_mode));



  // if(vision_indexer->Get_Object().width > 300)
  // {
    // lcd::set_text(3, to_string(vision_indexer->Get_Object().signature) + "width" + to_string(vision_indexer->Get_Object().width) + " " + to_string(vision_indexer->vision_sensor->get_by_sig(0,2).width));
  // }
  // angler->Smooth_Angler(master.get_digital(DIGITAL_X)*1.6);
  // angler->Auto_Angler(-2 * master.get_digital_new_press(DIGITAL_A));
  // arm->Arm_Macro(master.get_digital_new_press(DIGITAL_Y)*2 + master.get_digital_new_press(DIGITAL_B));
  // arm->Increment_Arm((master.get_digital(DIGITAL_R1) - master.get_digital(DIGITAL_R2)));

  // angler->Override_Mode(2* master.get_digital_new_press(DIGITAL_X) - master.get_digital(DIGITAL_X));

  // if (master.get_digital_new_press(DIGITAL_LEFT))
  // { 
  //   snapshot = !snapshot;
  // }
  // if(!snapshot)
  // {
    // lcd::set_text(1, "lHeat" + to_string((int)intakeMotorLeft.get_temperature()) + " rHeat" + to_string((int)intakeMotorRight.get_temperature()) + " refHeat" + to_string((int)left_back_motor.get_temperature()));
    // lcd::set_text(2, "lEff" + to_string((int)intakeMotorLeft.get_efficiency()) + " rEff" + to_string((int)intakeMotorRight.get_efficiency()) );
    // lcd::set_text(3, "lVolt" + to_string((int)intakeMotorLeft.get_voltage()) + " rVolt" + to_string((int)intakeMotorRight.get_voltage()));
    // lcd::set_text(4, "lVel" + to_string((int)intakeMotorLeft.get_actual_velocity()) + " rVel" + to_string((int)intakeMotorRight.get_actual_velocity()));
    // lcd::set_text(5, "lCur" + to_string((int)intakeMotorLeft.get_current_draw()) + " rCur" + to_string((int)intakeMotorRight.get_current_draw()));

    // lcd::set_text(1, "lHeat" + to_string((int)arm_motor.get_temperature()) + " rHeat" + to_string((int)intakeMotorRight.get_temperature()) + " refHeat" + to_string((int)left_back_motor.get_temperature()));
    // lcd::set_text(2, "lEff" + to_string((int)arm_motor.get_efficiency()) + " rEff" + to_string((int)intakeMotorRight.get_efficiency()));
    // lcd::set_text(3, "lVolt" + to_string((int)arm_motor.get_voltage()) + " rVolt" + to_string((int)intakeMotorRight.get_voltage()));
    // lcd::set_text(4, "lVel" + to_string((int)arm_motor.get_actual_velocity()) + " rVel" + to_string((int)intakeMotorRight.get_actual_velocity()));
    // lcd::set_text(5, "lCur" + to_string((int)arm_motor.get_current_draw()) + " rCur" + to_string((int)intakeMotorRight.get_current_draw()));
  // }
  // ultra_finder->Ultra_Angle();
  
  // bot_overheating();


  // arm->Set_Target(arm->Get_Target() - 40*master.get_digital(DIGITAL_UP));

  //  pros::lcd::set_text(1, "Target height" + to_string(angler->Get_Height()));
}

void opcontrol() {

    while (true)
    {
      master_control->run();
      pros::delay(DELAY_INTERVAL);  
    } 
}

	
// bool overheating;
// int overheat_temp = 45;
// int last_overheat = -10000000;
// string overheating_motors = "";
//   void bot_overheating() {
//       overheating = false;
//       overheating_motors = "High Temp: ";
//       if (left_front_motor.get_temperature() >= overheat_temp)
//       {
//         overheating = true;
//         overheating_motors += "LFD" + to_string((int)left_front_motor.get_temperature())+ ", ";
//       }
//       if (left_back_motor.get_temperature() >= overheat_temp)
//       {
//         overheating = true;
//         overheating_motors += "LBD" + to_string((int)left_back_motor.get_temperature()) + ", ";
//       }
//       if (right_front_motor.get_temperature() >= overheat_temp)
//       {
//         overheating = true;
//         overheating_motors += "RFD" + to_string((int)right_front_motor.get_temperature()) + ", ";
//       }
//       if (right_back_motor.get_temperature() >= overheat_temp)
//       {
//         overheating = true;
//         overheating_motors += "RBD" + to_string((int)right_back_motor.get_temperature()) + ", ";
//       }
//       if (angler_motor.get_temperature() >= overheat_temp)
//       {
//         overheating = true;
//         overheating_motors += "Ang" + to_string((int)angler_motor.get_temperature()) + ", ";
//       }
//       if (arm_motor.get_temperature() >= overheat_temp)
//       {
//         overheating = true;
//         overheating_motors += "Arm" + to_string((int)arm_motor.get_temperature()) + ", ";
//       }
//       if (intakeMotorLeft.get_temperature() >= overheat_temp)
//       {
//         overheating = true;
//         overheating_motors += "L In" + to_string((int)intakeMotorLeft.get_temperature()) + ", ";
//       }
//       if (intakeMotorRight.get_temperature() >= overheat_temp)
//       {
//         overheating = true;
//         overheating_motors += "R In" + to_string((int)intakeMotorRight.get_temperature()) + ", ";
//       }
//       if (overheating)
//       {
//         if (pros::millis() - last_overheat > 60000 && !Competition_Env)
//         {
//           last_overheat = pros::millis(); 
//           master.rumble("- . .");

          
//         }
//         char cstr[overheating_motors.size() + 1];
//         strcpy(cstr, overheating_motors.c_str());
//         master.print(0, 0, cstr);
//         lcd::set_text(6,overheating_motors);
//     }
//   }