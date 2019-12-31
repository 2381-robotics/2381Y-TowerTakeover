#include "robot/modules/drive/mech_drive.hpp"
#include "robot/control/motor_controller.hpp"
#include "api.h"
#include "globals.hpp"
#include <array>
#include <math.h> /* pow */
#include "main.h"
#include "opcontrol.h"
#include "ports.h"

#include "pros/misc.h"

using namespace std;
using namespace pros;

double Mech_Drive::Get_Speed()
{
  return (abs(_left_front_motor_controller->Get_Speed()) + abs(_left_back_motor_controller->Get_Speed()) + abs(_right_back_motor_controller->Get_Speed()) + abs(_right_front_motor_controller->Get_Speed())) / 4;
}

void Mech_Drive::Set_Init_Point()
{
  std::array<double, 4> tempArr = {_left_front_motor_controller->Get_Distance(), _left_back_motor_controller->Get_Distance(), _right_front_motor_controller->Get_Distance(), _right_back_motor_controller->Get_Distance()};
  initial_position = tempArr;
}
bool Mech_Drive::get_running()
{
  return _is_running;
}
void Mech_Drive::Reset_Point()
{
  Set_Init_Point();
  // Drive_Distance_Controller->ResetError();
  _is_running = true;
}

double Mech_Drive::Get_Distance()
{
  return (std::abs(_left_front_motor_controller->Get_Distance() - initial_position[0]) + std::abs(_left_back_motor_controller->Get_Distance() - initial_position[1]) + std::abs(_right_front_motor_controller->Get_Distance() - initial_position[2]) + std::abs(_right_back_motor_controller->Get_Distance() - initial_position[3])) / 4;
}

double trollCalc(double masterDis, double masterOS, double specDis, double specOS)
{
  if (masterOS == 0 || specDis == 0 || masterDis == 0 || specOS == 0)
  {
    return 1;
  }
  return (masterDis * specOS / (masterOS * specDis));
}

void Mech_Drive::Set_Drive(double left_x, double left_y, double right_x, double right_y)
{
  // pros::lcd::set_text(1, _left_back_motor_controller->Get_Speed);
  _left_back_setpoint = (left_y - left_x + std::abs(right_x) * (right_x) / 127);
  _left_front_setpoint = (left_y + left_x + std::abs(right_x) * (right_x) / 127);
  _right_back_setpoint = (left_y + left_x - std::abs(right_x) * (right_x) / 127);
  _right_front_setpoint = (left_y - left_x - std::abs(right_x) * (right_x) / 127);

  _master_setpoint = (abs(_left_back_setpoint) + abs(_left_front_setpoint) + abs(_right_back_setpoint) + abs(_right_front_setpoint)) / 4;
  _master_offset += (_master_setpoint);
  // pros::lcd::set_text(0, "master setpoint integral - " + to_string(_master_offset));
  rfoffset += (abs(_right_front_setpoint));
  lfoffset += (abs(_left_front_setpoint));
  rboffset += (abs(_right_back_setpoint));
  lboffset += (abs(_left_back_setpoint));

  // pros::lcd::set_text(1, "right front offset - " + to_string(rfoffset));
  rfDistance += abs(_right_front_motor_controller->Get_Speed()) / DELAY_INTERVAL;
  lfDistance += abs(_left_front_motor_controller->Get_Speed()) / DELAY_INTERVAL;
  rbDistance += abs(_right_back_motor_controller->Get_Speed()) / DELAY_INTERVAL;
  lbDistance += abs(_left_back_motor_controller->Get_Speed()) / DELAY_INTERVAL;

  // pros::lcd::set_text(4, "rightDistance" + to_string(rfDistance + rbDistance));
  // pros::lcd::set_text(3, "leftDistance" + to_string(lfDistance + lbDistance));

  masterDistance += (abs(_right_front_motor_controller->Get_Speed()) + abs(_left_back_motor_controller->Get_Speed()) + abs(_right_back_motor_controller->Get_Speed()) + abs(_left_front_motor_controller->Get_Speed())) / (4 * DELAY_INTERVAL);
  // lcd::set_text(3, "calcVal" + to_string(masterDistance * rfoffset - rfDistance * _master_offset));
  double calcValue = masterDistance * rfoffset - rfDistance * _master_offset;

  // pros::lcd::set_text(0, to_string(this->Get_Speed()) +  "Speed ");
  double tuning_coefficient = _master_pid->Update(0, _master_error_average);

  _left_back_motor_value = _left_back_motor_controller->Set_Speed(_left_back_setpoint * tuning_coefficient * trollCalc(masterDistance, _master_offset, lbDistance, lboffset));
  _left_front_motor_value = _left_front_motor_controller->Set_Speed(_left_front_setpoint * tuning_coefficient * trollCalc(masterDistance, _master_offset, lfDistance, lfoffset));
  _right_back_motor_value = _right_back_motor_controller->Set_Speed(_right_back_setpoint * tuning_coefficient * trollCalc(masterDistance, _master_offset, rbDistance, rboffset));
  _right_front_motor_value = _right_front_motor_controller->Set_Speed(_right_front_setpoint * tuning_coefficient * trollCalc(masterDistance, _master_offset, rfDistance, rfoffset));

  // if (master.get_digital(E_CONTROLLER_DIGITAL_X))
  // {
  //   lcd::set_text(0, to_string(lboffset) + "leftback");
  //   lcd::set_text(1, to_string(rboffset) + "right back");
  //   lcd::set_text(2, to_string(lfoffset)  + "l front");
  //   lcd::set_text(3, to_string(rfoffset) + "r front");

  //   // lcd::set_text(4, to_string(_master_offset) + "master");
  //   lcd::set_text(5, to_string(trollCalc(_right_front_motor_controller->Get_Speed(), _right_front_setpoint)) + "master");
  // }

  // // _master_offset = pow((trollCalc(_left_back_motor_controller->Get_Speed(), _left_back_setpoint) * trollCalc(_right_back_motor_controller->Get_Speed(), _right_back_setpoint) * trollCalc(_left_front_motor_controller->Get_Speed(), _left_front_setpoint) * trollCalc(_right_front_motor_controller->Get_Speed(), _right_front_setpoint)), 0.25);
  // _master_offset = ((trollCalc(_left_back_motor_controller->Get_Speed(), _left_back_setpoint) + trollCalc(_right_back_motor_controller->Get_Speed(), _right_back_setpoint) + trollCalc(_left_front_motor_controller->Get_Speed(), _left_front_setpoint) + trollCalc(_right_front_motor_controller->Get_Speed(), _right_front_setpoint))* 0.25);
  // lboffset += trollCalc(_left_back_motor_controller->Get_Speed(), _left_back_setpoint) - _master_offset;
  // rboffset += trollCalc(_right_back_motor_controller->Get_Speed(), _right_back_setpoint) - _master_offset;
  // lfoffset += trollCalc(_left_front_motor_controller->Get_Speed(), _left_front_setpoint) - _master_offset;
  // rfoffset += trollCalc(_right_front_motor_controller->Get_Speed(), _right_front_setpoint) - _master_offset;

  _motor_value_average = (abs(_left_back_motor_value) + abs(_left_front_motor_value) + abs(_right_back_motor_value) + abs(_right_front_motor_value)) / 4;
  if (_master_setpoint >= 0)
  {
    _master_error_average = _motor_value_average - _master_setpoint;
  }
  else
  {
    _master_error_average = _master_setpoint - _motor_value_average;
  }
}

std::array<double, 2> Mech_Drive::Convert(double speed, double direction)
{
  double x = cos(direction * (3.14159 / 180)) * speed;
  double y = sin(direction * (3.14159 / 180)) * speed;
  std::array<double, 2> drive_coordinates = {x, y};
  return drive_coordinates;
}

void Mech_Drive::Set_Turn(double speed, double direction, double distance)
{
  std::array<double, 2> drive_convert = Convert(speed, direction);

  double actualDistance = this->Get_Distance();

  if (actualDistance < distance)
  {
    Set_Drive(0, 0, speed, 0);
    _is_running = true;
  }
  else
  {
    Set_Drive(0, 0, 0, 0);
    _is_running = false;
  }
}
void Mech_Drive::Set_Point_Drive(double speed, double direction, double distance, double turnSpeed, double accelScaling)
{

  std::array<double, 2> drive_convert = Convert(speed, direction);
  double actualDistance = this->Get_Distance();

  // double distanceControlCoeff = this->Drive_Distance_Controller->Update(0, (actualDistance - distance) / std::abs(distance));
  // pros::lcd::set_text(4, "distance Control : " + to_string(this->Drive_Distance_Controller->error_sum_*0.0015));

  // pros::lcd::set_text(0, to_string(_left_back_motor_controller->Get_Distance()) + "leftback");
  // pros::lcd::set_text(3, to_string(_left_front_motor_controller->Get_Distance()) + "left front");

  if (std::abs(actualDistance - distance) > distance / 100 + 15 || this->Get_Speed() > 20)
  {
    pros::lcd::set_text(3, ":" + to_string(Get_Distance()) + "distance away " + to_string(abs(actualDistance - distance)));

    if (std::abs(actualDistance) > distance + 400)
      {
        // this->Reset_Point
        Set_Drive(0, 0, 0, 0);
        return;
        pros::lcd::set_text(0, "drive exceed" + to_string(actualDistance) + ">" + to_string(distance));
      }

    double leftX = drive_convert[1];
    double leftY = drive_convert[0];
    double rightX = turnSpeed;
    if (std::abs(actualDistance - distance) < (distance * speed / 127) / 4)
    {
      double deaccelCoeff = pow(std::abs(actualDistance - distance) / ((distance * speed / 127) / 4), (speed / (127 * accelScaling)));
      leftX = leftX * deaccelCoeff;
      leftY = leftY * deaccelCoeff;
    }
    if (std::abs(actualDistance - distance) < (distance * abs(turnSpeed) / 127) / 4)
    {
      double deaccelTurnCoeff = pow(std::abs(actualDistance - distance) / ((distance * abs(turnSpeed) / 127) / 4), (abs(turnSpeed) / (127 * accelScaling)));
      rightX = rightX*deaccelTurnCoeff;
    }
    if (std::abs(actualDistance) < (distance * speed / 127) / 4){
      double accelCoeff = pow((abs(actualDistance) + (distance * speed / 127) / 8) / ((3 * distance * speed / 127) / 8), speed / (256 * accelScaling));
      leftX = leftX * accelCoeff;
      leftY = leftY * accelCoeff;
    }
    if (std::abs(actualDistance) < (distance * abs(turnSpeed) / 127) / 4)
    {

      double accelCoeff = pow((abs(actualDistance) + (distance * abs(turnSpeed) / 127) / 8) / ((3 * distance * abs(turnSpeed) / 127) / 8), abs(turnSpeed) / (256 * accelScaling));
      lcd::set_text(0, "turn Accel +"  + to_string(accelCoeff));
      rightX  = rightX  * accelCoeff;
    }

    // if (std::abs(actualDistance - distance) < (distance * speed / 127) / 4)
    // {
    //   double deaccelCoeff = pow(std::abs(actualDistance - distance) / ((distance * speed / 127) / 4), (speed / (127 * accelScaling)));
    //   double deaccelTurnCoeff = pow(std::abs(actualDistance - distance) / ((distance * turnSpeed / 127) / 4), (turnSpeed / (127 * accelScaling)));
    //   Set_Drive(drive_convert[1] * deaccelCoeff, drive_convert[0] * deaccelCoeff, turnSpeed * deaccelTurnCoeff, 0);
    //   pros::lcd::set_text(0, "slowing down " + to_string(deaccelCoeff));
    //   }
    //   else if (std::abs(actualDistance) < (distance * speed / 127) / 4)
    //   {
    //     double accelCoeff = pow((abs(actualDistance) + (distance * speed / 127) / 8) / ((3 * distance * speed / 127) / 8), speed / (256 * accelScaling));
    //     double accelTurnCoeff = pow((abs(actualDistance) + (distance * turnSpeed / 127) / 8) / ((3 * distance * turnSpeed / 127) / 8), turnSpeed / (256 * accelScaling));
    //     Set_Drive(drive_convert[1] * accelCoeff, drive_convert[0] * accelCoeff, turnSpeed * accelTurnCoeff, 0);
    //     // pros::lcd::set_text(0, "y drive value " + to_string(accelCoeff * drive_convert[0]));
    //   }
    Set_Drive(leftX, leftY , rightX, 0);
    _is_running = true;
  }
  else
  {
    // Set_Drive(0, 0, 0, 0);
    Stop();
    // pros::lcd::set_text(4, "HELLO THERE" + to_string(distance));
    _is_running = false;
  }
}

void Mech_Drive::Stop()
{
  left_front_motor.move(0);
  right_front_motor.move(0);
  right_back_motor.move(0);
  left_back_motor.move(0);
}
void Mech_Drive::create()
{
  // Drive_Distance_Controller = new Pid(&distance_controller_pid_values[0], &distance_controller_pid_values[1], &distance_controller_pid_values[2]);
  std::array<double, 4>
      tempArray = {0, 0, 0, 0};
  initial_position = {0, 0, 0, 0};
  _left_front_motor_controller = new Motor_Controller(&left_front_pid_values[0], &left_front_pid_values[1], &left_front_pid_values[2], &left_front_motor);
  _left_back_motor_controller = new Motor_Controller(&left_back_pid_values[0], &left_back_pid_values[1], &left_back_pid_values[2], &left_back_motor);
  _right_front_motor_controller = new Motor_Controller(&right_front_pid_values[0], &right_front_pid_values[1], &right_front_pid_values[2], &right_front_motor);
  _right_back_motor_controller = new Motor_Controller(&left_front_pid_values[0], &left_front_pid_values[1], &left_front_pid_values[2], &right_back_motor);
  _master_pid = new Pid(&(master_drive_pid_values)[0], &(master_drive_pid_values)[1], &(master_drive_pid_values)[2]);
  _master_error_average = 0;
  _master_setpoint = 1;
}
//Empty default constructor for blank factory arguments.
Mech_Drive::Mech_Drive()
{
}
