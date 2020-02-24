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
#include "globals.hpp"

using namespace pros;

// Motor testMotorLeft (1, true);
// Motor testMotorRight (10, false);

//   Intake * intake = new Intake();
int anglerGoingDown = 0;
bool snapshot = false;
void driver()  {
  intake->Set_Intake((master.get_digital(DIGITAL_L1) * 127 - master.get_digital(DIGITAL_L2) * 90 - master.get_digital(DIGITAL_Y) * 30 ));
  robot->set_drive(master.get_analog(ANALOG_LEFT_X), master.get_analog(ANALOG_LEFT_Y), master.get_analog(ANALOG_RIGHT_X), master.get_analog(ANALOG_RIGHT_Y));
  /*done just upload*/
  // intakeMotorLeft.move_voltage(master.get_digital(DIGITAL_L1)* 12000 - master.get_digital(DIGITAL_L2) * 6000);
  // intakeMotorRight.move_voltage(master.get_digital(DIGITAL_L1) * 12000 - master.get_digital(DIGITAL_L2) * 6000);

  angler->Smooth_Angler(master.get_digital(DIGITAL_X) + master.get_digital(DIGITAL_B)*0.8);
  angler->Auto_Angler(-2 * master.get_digital_new_press(DIGITAL_A));
  // angler->Override_Mode(2* master.get_digital_new_press(DIGITAL_X) - master.get_digital(DIGITAL_X));

  if (master.get_digital_new_press(DIGITAL_UP))
  { 
    snapshot = !snapshot;
  }
  if(!snapshot)
  {
    // lcd::set_text(1, "lHeat" + to_string((int)intakeMotorLeft.get_temperature()) + " rHeat" + to_string((int)intakeMotorRight.get_temperature()) + " refHeat" + to_string((int)left_back_motor.get_temperature()));
    // lcd::set_text(2, "lEff" + to_string((int)intakeMotorLeft.get_efficiency()) + " rEff" + to_string((int)intakeMotorRight.get_efficiency()) );
    // lcd::set_text(3, "lVolt" + to_string((int)intakeMotorLeft.get_voltage()) + " rVolt" + to_string((int)intakeMotorRight.get_voltage()));
    // lcd::set_text(4, "lVel" + to_string((int)intakeMotorLeft.get_actual_velocity()) + " rVel" + to_string((int)intakeMotorRight.get_actual_velocity()));
    // lcd::set_text(5, "lCur" + to_string((int)intakeMotorLeft.get_current_draw()) + " rCur" + to_string((int)intakeMotorRight.get_current_draw()));

    lcd::set_text(1, "lHeat" + to_string((int)angler_motor.get_temperature()) + " rHeat" + to_string((int)intakeMotorRight.get_temperature()) + " refHeat" + to_string((int)left_back_motor.get_temperature()));
    lcd::set_text(2, "lEff" + to_string((int)angler_motor.get_efficiency()) + " rEff" + to_string((int)intakeMotorRight.get_efficiency()));
    lcd::set_text(3, "lVolt" + to_string((int)angler_motor.get_voltage()) + " rVolt" + to_string((int)intakeMotorRight.get_voltage()));
    lcd::set_text(4, "lVel" + to_string((int)angler_motor.get_actual_velocity()) + " rVel" + to_string((int)intakeMotorRight.get_actual_velocity()));
    lcd::set_text(5, "lCur" + to_string((int)angler_motor.get_current_draw()) + " rCur" + to_string((int)intakeMotorRight.get_current_draw()));
  }


  arm->Increment_Arm((master.get_digital(DIGITAL_R1) - master.get_digital(DIGITAL_R2)));
  

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