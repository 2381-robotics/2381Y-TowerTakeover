#include "robot/modules/drive/mech_drive.hpp"
#include "robot/control/motor_controller.hpp"
#include "api.h"
#include "globals.hpp"
#include <array>
#include <math.h> /* pow */
#include "main.h"
#include "opcontrol.h"
#include "ports.h"
#include "pros/api_legacy.h"

#include "pros/misc.h"

using namespace std;
using namespace pros;
// double notCurrentL, notCurrentR, notCurrentF, notChangeL, notChangeR, notChangeR, notChangeF, notLastL, notLastR, notLastF;

// void trackLocalPos(int offsetL, int offsetR, int offsetF)
// {
//   notCurrentL = pros::c::encoderGet(left) - offsetL;
//   notCurrentR = pros::c::encoderGet(right) - offsetR;
//   notCurrentF = pros::c::encoderGet(back) - offsetF;

//   notChangeL = (notCurrentL - notLastL) * PI * WHEEL_DIAMETER / 360; // The amount the left side of the robot moved
//   notChangeR = (notCurrentR - notLastR) * PI * WHEEL_DIAMETER / 360; // The amount the right side of the robot moved
//   notChangeF = (notCurrentF - notLastF) * PI * WHEEL_DIAMETER / 360; // The amount the front of the robot moved
//   //update last values
//   notLastL = notCurrentL;
//   notLastR = notCurrentR;
//   notLastF = notCurrentF;

//   float hyp;                                                       //hypotenuse of triangle (starting center, end center, middle of circle it goes around)
//   float i;                                                         //angle travelled * 0.5
//   float h2;                                                        //same as hyp but using the back instead of the side wheels
//   float angle = (notChangeL - notChangeR) / (L_TO_MID + R_TO_MID); //angle travelled
//   if (angle != 0)
//   {
//     float radius = notChangeR / angle; // The radius of the circle the robot travel's around with the right side of the robot
//     i = angle / 2.0;
//     float sinI = sin(i);
//     hyp = ((radius + R_TO_MID) * sinI) * 2.0;
//     float r2 = notChangeF / angle; // The radius of the circle the robot travel's around with the back of the robot
//     h2 = ((r2 + F_TO_MID) * sinI) * 2.0;
//   }
//   else
//   {
//     hyp = notChangeR;
//     i = 0;
//     h2 = notChangeF;
//   }
//   float p = i + localTheta; //ending angle
//   float cosP = cos(p);
//   float sinP = sin(p);

//   //local position and angle update
//   localPosY += hyp * cosP;
//   localPosX += hyp * sinP;
//   localPosY += h2 * -sinP;
//   localPosX += h2 * cosP;
//   localTheta += angle;
// }

void Mech_Drive::Move_Motor() {


    _left_back_motor_value = _left_back_motor_controller->Set_Speed(_pid_inputs[left_back]);
    _left_front_motor_value = _left_front_motor_controller->Set_Speed(_pid_inputs[left_front]);
    _right_back_motor_value = _right_back_motor_controller->Set_Speed(_pid_inputs[right_back]);
    _right_front_motor_value = _right_front_motor_controller->Set_Speed(_pid_inputs[right_front]);
}

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

void Mech_Drive::Move_Wheel(double speed) {
  _pid_inputs[left_front]  = speed;
_pid_inputs[left_back] = speed;
}
//  
double trollCalc(double masterDis, double masterOS, double specDis, double specOS)
{
  if (masterOS == 0 || specDis == 0 || masterDis == 0 || specOS == 0)
  {
    return 1;
  }
  return pow((masterDis * specOS / (masterOS * specDis)),2);
}

// Desired Parallel Velocity Magnitude, Desired Perpindicular Velocity Magnitude, Desired Angular Velocity Magnitudes
// Issues: Strafing - turns for some fucking reason, strafes in general are kinda inconsistent, 
// Driving straight and stopping turns the robot sometimes, position is wack.




void Mech_Drive::Set_Drive(double left_x , double left_y, double right_x, double right_y)
{
  _motor_value_average = (abs(_left_back_motor_value) + abs(_left_front_motor_value) + abs(_right_back_motor_value) + abs(_right_front_motor_value)) / 4;
  if (_master_setpoint >= 0)
  {
    _master_error_average = _motor_value_average - _master_setpoint;
  }
  else
  {
    _master_error_average = _master_setpoint - _motor_value_average;
  }
  _left_back_setpoint = (left_y - left_x + std::abs(right_x) * (right_x) / 127);
  _left_front_setpoint = (left_y + left_x + std::abs(right_x) * (right_x) / 127);
  _right_back_setpoint = (left_y + left_x - std::abs(right_x) * (right_x) / 127);
  _right_front_setpoint = (left_y - left_x - std::abs(right_x) * (right_x) / 127);

  _master_setpoint = (abs(_left_back_setpoint) + abs(_left_front_setpoint) + abs(_right_back_setpoint) + abs(_right_front_setpoint)) / 4;

  _master_offset += (_master_setpoint);
  lfoffset += (abs(_left_front_setpoint));
  rboffset += (abs(_right_back_setpoint));
  lboffset += (abs(_left_back_setpoint));
  rfoffset += (abs(_right_front_setpoint));


  // pros::lcd::set_text(1, "right front offset - " + to_string(rfoffset));
  rfDistance += abs(_right_front_motor_controller->Get_Speed()) / DELAY_INTERVAL;
  lfDistance += abs(_left_front_motor_controller->Get_Speed()) / DELAY_INTERVAL;
  rbDistance += abs(_right_back_motor_controller->Get_Speed()) / DELAY_INTERVAL;
  lbDistance += abs(_left_back_motor_controller->Get_Speed()) / DELAY_INTERVAL;


  masterDistance += (abs(_right_front_motor_controller->Get_Speed()) + abs(_left_back_motor_controller->Get_Speed()) + abs(_right_back_motor_controller->Get_Speed()) + abs(_left_front_motor_controller->Get_Speed())) / (4 * DELAY_INTERVAL);

  // pros::lcd::set_text(0, to_string(this->Get_Speed()) +  "Speed ");
  double tuning_coefficient = _master_pid->Update(0, _master_error_average);

  _pid_inputs[left_back] = _left_back_setpoint * tuning_coefficient * trollCalc(masterDistance, _master_offset, lbDistance, lboffset);
  _pid_inputs[left_front] = _left_front_setpoint * tuning_coefficient * trollCalc(masterDistance, _master_offset, lfDistance, lfoffset);
  _pid_inputs[right_back] = _right_back_setpoint * tuning_coefficient * trollCalc(masterDistance, _master_offset, rbDistance, rboffset);
  _pid_inputs[right_front] = _right_front_setpoint * tuning_coefficient * trollCalc(masterDistance, _master_offset, rfDistance, rfoffset);

  // if (master.get_digital(E_CONTROLLER_DIGITAL_X))
  // {
  //   lcd::set_text(0, to_string(lboffset) + "leftback");
  //   lcd::set_text(1, to_string(rboffset) + "right back");
  //   lcd::set_text(2, to_string(lfoffset)  + "l front");
  //   lcd::set_text(3, to_string(rfoffset) + "r front");

  //   // lcd::set_text(4, to_string(_master_offset) + "master");
  lcd::set_text(6, to_string((float)trollCalc(masterDistance, _master_offset, rbDistance, rboffset))  + " bk:" + to_string((float)trollCalc(masterDistance, _master_offset, rfDistance, rfoffset)));
  // }

  // // _master_offset = pow((trollCalc(_left_back_motor_controller->Get_Speed(), _left_back_setpoint) * trollCalc(_right_back_motor_controller->Get_Speed(), _right_back_setpoint) * trollCalc(_left_front_motor_controller->Get_Speed(), _left_front_setpoint) * trollCalc(_right_front_motor_controller->Get_Speed(), _right_front_setpoint)), 0.25);
  // _master_offset = ((trollCalc(_left_back_motor_controller->Get_Speed(), _left_back_setpoint) + trollCalc(_right_back_motor_controller->Get_Speed(), _right_back_setpoint) + trollCalc(_left_front_motor_controller->Get_Speed(), _left_front_setpoint) + trollCalc(_right_front_motor_controller->Get_Speed(), _right_front_setpoint))* 0.25);

  
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
void Mech_Drive::Set_Point_Drive(double speed, double direction, double distance, double turnSpeed, double accelScaling, bool blocking, double criticalPoint, double criticalMultiplier)
{

  std::array<double, 2> drive_convert = Convert(speed, direction);
  double actualDistance = this->Get_Distance();

  // double distanceControlCoeff = this->Drive_Distance_Controller->Update(0, (actualDistance - distance) / std::abs(distance));
  // pros::lcd::set_text(4, "distance Control : " + to_string(this->Drive_Distance_Controller->error_sum_*0.0015));

  // pros::lcd::set_text(0, to_string(_left_back_motor_controller->Get_Distance()) + "leftback");
  // pros::lcd::set_text(3, to_string(_left_front_motor_controller->Get_Distance()) + "left front");
  if (std::abs(actualDistance - distance) >( distance / 50 + 100)* criticalMultiplier || this->Get_Speed() > 20)
  {
    pros::lcd::set_text(3, ":" + to_string((int)Get_Distance()) + "distance away " + to_string((int)abs(actualDistance - distance)) + "critical" + to_string((int)distance/50 + 15));

    if (std::abs(actualDistance) > distance + criticalPoint)
      { 
        // this->Reset_Point
        Stop();
        pros::lcd::set_text(0, "drive exceed" + to_string(actualDistance) + ">" + to_string(distance));
        if(blocking){
          return;
        }
        _is_running = false;
  
      }

    double leftX = drive_convert[1];
    double leftY = drive_convert[0];
    double rightX = turnSpeed;
    if (std::abs(actualDistance - distance) < (distance * speed / 127) / 4)
    {
      double deaccelCoeff = pow(std::abs(actualDistance - distance) / ((distance * speed / 127) / 4), (speed / (127 * accelScaling))) * abs(actualDistance - distance) / (distance-actualDistance);
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

    Set_Drive(leftX, leftY , rightX, 0);
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

//Empty default constructor for blank factory arguments.
Mech_Drive::Mech_Drive()
{
 
}

void Mech_Drive::Create() {
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