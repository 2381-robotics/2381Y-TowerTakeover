#include "robot/modules/drive/mech_drive.hpp"
#include "robot/control/motor_controller.hpp"
#include "api.h"
#include "globals.hpp"
#include <array>
#include <math.h> /* pow */
#include <numeric>
#include "main.h"
#include "opcontrol.h"
#include "ports.h"
#include "pros/api_legacy.h"
#include "position_tracker.hpp"
#include "pros/misc.h"
#include "legacy_globals.hpp"

using namespace std;
using namespace pros;

double NormalizeAngle(double angle)
{
  return fmod(angle + M_PI, 2 * M_PI) - M_PI;
}

void Mech_Drive::Move_Motor()
{

  _left_back_motor_value = _left_back_motor_controller->Set_Speed(_pid_inputs[left_back]);
  _left_front_motor_value = _left_front_motor_controller->Set_Speed(_pid_inputs[left_front]);
  _right_back_motor_value = _right_back_motor_controller->Set_Speed(_pid_inputs[right_back]);
  _right_front_motor_value = _right_front_motor_controller->Set_Speed(_pid_inputs[right_front]);

  // _left_back_motor_value = left_back_motor.move(_pid_inputs[left_back]);
  // _left_front_motor_value = left_front_motor.move(_pid_inputs[left_front]);
  // _right_back_motor_value = right_back_motor.move(_pid_inputs[right_back]);
  // _right_front_motor_value = right_front_motor.move(_pid_inputs[right_front]);
}

auto TurnControl = new Pid({-1, 0, 0});

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
  started = pros::c::millis();
  Set_Init_Point();
  _is_running = true;
}

double Mech_Drive::Get_Distance()
{
  return (std::abs(_left_front_motor_controller->Get_Distance() - initial_position[0]) + std::abs(_left_back_motor_controller->Get_Distance() - initial_position[1]) + std::abs(_right_front_motor_controller->Get_Distance() - initial_position[2]) + std::abs(_right_back_motor_controller->Get_Distance() - initial_position[3])) / 4;
}

void Mech_Drive::Move_Wheel(double speed)
{
  _pid_inputs[left_front] = speed;
  _pid_inputs[left_back] = speed;
}

double ratioCalc(double masterDis, double masterOS, double specDis, double specOS)
{
  if (masterOS == 0 || specDis == 0 || masterDis == 0 || specOS == 0)
  {
    return 1;
  }
  return pow((masterDis * specOS / (masterOS * specDis)), 6);
}

// Desired Parallel Velocity Magnitude, Desired Perpindicular Velocity Magnitude, Desired Angular Velocity Magnitudes
// Issues: Strafing - turns for some fucking reason, strafes in general are kinda inconsistent,
// Driving straight and stopping turns the robot sometimes, position is wack.

double turningCoefficient = 1;

void Mech_Drive::Follow_Path()
{
  // array<double, 3> currentPosition = position_tracker->Get_Position();
  // double parDistance = sqrt(pow((currentPosition[0] - lookaheadPoint[0]), 2) + pow(currentPosition[1] - lookaheadPoint[1], 2));
  // float lookaheadAngle = atan2(lookaheadPoint[1], lookaheadPoint[0]);
  // float currentAngle = currentPosition[2];
  // float angleDifference = lookaheadAngle - currentAngle;
  // double perpendicularDistance = parDistance * sin(angleDifference) / sin(PI - 2 * parDistance);
  // double curvature = perpendicularDistance == 0 ? 256 : 1 / perpendicularDistance;
  // Set_Drive(0, 60, curvature * turningCoefficient, 0);
}

void Mech_Drive::Set_Drive(double left_x, double left_y, double right_x, double right_y)
{

  _motor_value_average = (abs(_left_back_motor_value) + abs(_left_front_motor_value) + abs(_right_back_motor_value) + abs(_right_front_motor_value)) / 4;
  //motor_value_average is what the actual motors are currently set at

  if (_master_setpoint >= 0) //setpoint is what we as controllers want the code to actually output
  {
    _master_error_average = _motor_value_average - _master_setpoint; //master error is used in the pid values to tune the motor values
  }
  else
  {
    _master_error_average = _master_setpoint - _motor_value_average;
  }
  _left_back_setpoint = (left_y - left_x + pow((std::abs(right_x) / 127), 0.8) * (right_x));
  _left_front_setpoint = (left_y + left_x + pow((std::abs(right_x) / 127), 0.8) * (right_x));
  _right_back_setpoint = (left_y + left_x - pow((std::abs(right_x) / 127), 0.8) * (right_x));
  _right_front_setpoint = (left_y - left_x - pow((std::abs(right_x) / 127), 0.8) * (right_x));

  _master_setpoint = (abs(_left_back_setpoint) + abs(_left_front_setpoint) + abs(_right_back_setpoint) + abs(_right_front_setpoint)) / 4;
  _master_offset += (_master_setpoint);

  lfoffset += (abs(_left_front_setpoint));
  rboffset += (abs(_right_back_setpoint));
  lboffset += (abs(_left_back_setpoint));
  rfoffset += (abs(_right_front_setpoint));

  rfDistance += abs(_right_front_motor_controller->Get_Speed()) / DELAY_INTERVAL;
  lfDistance += abs(_left_front_motor_controller->Get_Speed()) / DELAY_INTERVAL;
  rbDistance += abs(_right_back_motor_controller->Get_Speed()) / DELAY_INTERVAL;
  lbDistance += abs(_left_back_motor_controller->Get_Speed()) / DELAY_INTERVAL;

  masterDistance += (abs(_right_front_motor_controller->Get_Speed()) + abs(_left_back_motor_controller->Get_Speed()) + abs(_right_back_motor_controller->Get_Speed()) + abs(_left_front_motor_controller->Get_Speed())) / (4 * DELAY_INTERVAL);

  double tuning_coefficient = _master_pid->Update(0, _master_error_average);
  if (tuning_coefficient < 0)
  {
    _master_pid->ResetError();
    tuning_coefficient = 1;
  }

  _pid_inputs[left_back] = _left_back_setpoint * ratioCalc(masterDistance, _master_offset, lbDistance, lboffset) * tuning_coefficient;
  _pid_inputs[left_front] = _left_front_setpoint * ratioCalc(masterDistance, _master_offset, lfDistance, lfoffset) * tuning_coefficient;
  _pid_inputs[right_back] = _right_back_setpoint * ratioCalc(masterDistance, _master_offset, rbDistance, rboffset) * tuning_coefficient;
  _pid_inputs[right_front] = _right_front_setpoint * ratioCalc(masterDistance, _master_offset, rfDistance, rfoffset) * tuning_coefficient;

  if (master.get_digital(E_CONTROLLER_DIGITAL_A))
  {
    lcd::set_text(4, to_string((int)_pid_inputs[left_back]) + ":" + to_string((int)_pid_inputs[left_front]) + ":" + to_string((int)_pid_inputs[right_back]) + ":" + to_string((int)_pid_inputs[right_front]));
    lcd::set_text(5, to_string((int)_left_back_setpoint) + ":" + to_string((int)_left_front_setpoint) + ":" + to_string((int)_right_back_setpoint) + ":" + to_string((int)_right_front_setpoint));
    lcd::set_text(6, to_string((int)_left_back_motor_value) + ":" + to_string((int)_left_front_motor_value) + ":" + to_string((int)_right_back_motor_value) + ":" + to_string((int)_right_front_motor_value));
  }

  // lcd::set_text(5, to_string((int)_pid_inputs[left_back]) + ":" + to_string((int)_pid_inputs[left_front]) + ":" + to_string((int)_pid_inputs[right_back]) + ":" + to_string((int)_pid_inputs[right_front]));
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
int getSignOf(double yeet)
{
  if (yeet == 0)
  {
    return 0;
  }
  return (abs(yeet) / yeet);
}
array<double, 4> Mech_Drive::unstartedArray()
{
  return {444.4, 444.4, 0, 7923};
}
double previousAlign;

double UltraAlign()
{
  double leftDistance = ultra_left.get_value();
  double rightDistance = ultra_right.get_value();
  if (leftDistance == rightDistance)
  {
    return 0;
  }
  lcd::set_text(0, to_string(rightDistance) + "DIE" + to_string(leftDistance));
  double ultra = 1.5 * (abs(rightDistance - leftDistance) / (rightDistance - leftDistance) * 70 * pow(abs(rightDistance - leftDistance) / 70, 0.5));
  if (ultra != NAN && abs(ultra) < 500)
  {
    previousAlign = (ultra * 2 + previousAlign) / 3;
  }
  return previousAlign;
}

void Mech_Drive::Set_Curve_Drive(complex<double> Waypoint, double WaypointAngle, complex<double> EndPoint, double EndAngle, double speed, double curvature, double AngleInterpolation)
{
  auto CurrentPos = position_tracker->Get_Position();
  auto EndpointDisp = EndPoint - CurrentPos;
  auto WaypointDisplacement = Waypoint - CurrentPos;

  auto InnerAngle = arg(EndpointDisp) - arg(WaypointDisplacement);

  auto TotalDisp = EndpointDisp;

  if (cos(InnerAngle) <= 0)
  {
    _is_running = false;
    return;
  }

  TotalDisp *= sin(InnerAngle) * sin(InnerAngle);
  TotalDisp += (double)2 * WaypointDisplacement * cos(InnerAngle) * cos(InnerAngle) * curvature;

  auto TargetAngle = EndAngle + pow(cos(InnerAngle), AngleInterpolation) * NormalizeAngle(WaypointAngle-EndAngle);

  // lcd::set_text(3, "DISTANCE: " + to_string(abs(TotalDisp)));
  // lcd::set_text(4, "Current: " + to_string(CurrentPos.real()) + ", " + to_string(CurrentPos.imag()));
  // lcd::set_text(5, "End Point: " + to_string((TotalDisp+CurrentPos).real()) + ", " + to_string((TotalDisp+CurrentPos).imag()));
  // lcd::set_text(4, "Disp: " + to_string(TotalDisp.real()) + ", " + to_string(TotalDisp.imag()));

  auto AngleDisplacement = arg(TotalDisp);
  auto AngleRobot = (position_tracker->Get_Angle());

  auto AngleDiff = fmod(AngleRobot - AngleDisplacement + M_PI, 2 * M_PI) - M_PI;
  auto EndAngleDiff = (fmod((AngleRobot - TargetAngle) + M_PI, 2 * M_PI) - M_PI) / 2;

  double deaccellCoeff = abs(TotalDisp) * 127 / (9 * speed) < 1 ? abs(TotalDisp) * 127 / (9 * speed) : 1;

  auto Forwards = speed * cos(AngleDiff) * deaccellCoeff;
  auto Strafe = speed * sin(AngleDiff) * deaccellCoeff;
  auto Turn = speed * TurnControl->Update(0, (0.8 * sin(EndAngleDiff) / pow(pow(sin(EndAngleDiff), 2.0), 0.25) + 0.2 * abs(sin(EndAngleDiff)) / sin(EndAngleDiff)));

  // lcd::set_text(5, "AngleDiff: " + to_string((int)(AngleDiff*180/M_PI)) + "EndDiff: " + to_string((int)(EndAngleDiff*180/M_PI)));
  Set_Drive(Strafe, Forwards, Turn, 0);
}

void Mech_Drive::Set_Path_Drive(complex<double> EndPoint, double EndAngle, array<double, 2> speed, double errorTolerance)
{
  Set_Path_Drive(EndPoint, EndAngle, speed, {errorTolerance, errorTolerance});
}

void Mech_Drive::Set_Path_Drive(complex<double> EndPoint, double EndAngle, double speed, double errorTolerance)
{
  Set_Path_Drive(EndPoint, EndAngle, {speed, speed}, {errorTolerance, errorTolerance});
}

void Mech_Drive::Set_Path_Drive(complex<double> EndPoint, double EndAngle, double speed, array<double, 2> errorTolerance)
{
  Set_Path_Drive(EndPoint, EndAngle, {speed, speed}, errorTolerance);
}

void Mech_Drive::Set_Path_Drive(complex<double> EndPoint, double EndAngle, array<double, 2> speed, array<double, 2> errorTolerance)
{
  auto CurrentPos = position_tracker->Get_Position();
  auto Displacement = EndPoint - CurrentPos;

  lcd::set_text(3, "DISTANCE: " + to_string(abs(Displacement)));
  // lcd::set_text(4, "Current: " + to_string(CurrentPos.real()) + ", " + to_string(CurrentPos.imag()));
  // lcd::set_text(5, "End Point: " + to_string(EndPoint.real()) + ", " + to_string(EndPoint.imag()));
  lcd::set_text(4, "Disp: " + to_string(Displacement.real()) + ", " + to_string(Displacement.imag()));

  auto AngleDisplacement = arg(Displacement);
  auto AngleRobot = (position_tracker->Get_Angle());

  auto AngleDiff = fmod(AngleRobot - AngleDisplacement + M_PI, 2 * M_PI) - M_PI;
  //Normalize turn angle to between (-pi and pi) so sin(x/2) works properly

  auto EndAngleDiff = (fmod((AngleRobot - EndAngle) + M_PI, 2 * M_PI) - M_PI) / 2;

  if (abs(Displacement) < 1 * errorTolerance[0] && abs(EndAngleDiff) < 0.01 * errorTolerance[1])
  {
    Stop();
    _is_running = false;
    return;
  }

  double deaccellCoeff = abs(Displacement) * 127 / (9 * speed[0]) < 1 ? abs(Displacement) * 127 / (9 * speed[0]) : 1;
  double deaccelSecond = abs(Displacement) * 127 / (6 * speed[0]) < 1 ? abs(Displacement) * 127 / (6 * speed[0]) : 1;

  // double deaccellCoeff = 1;

  // TurnControl->Update(0, sin(EndAngleDiff));
  auto Forwards = speed[0] * cos(AngleDiff) * deaccelSecond;
  auto Strafe = speed[0] * sin(AngleDiff) * deaccelSecond;
  auto Turna = (0.8 * sin(EndAngleDiff) / pow(pow(sin(EndAngleDiff), 2.0), 0.25) + 0.2 * abs(sin(EndAngleDiff)) / sin(EndAngleDiff));

  auto Turn = speed[1] * ((abs(EndAngleDiff) < 0.5 * errorTolerance[1] || deaccellCoeff / errorTolerance[1] > 1) ? Turna : Turna * deaccellCoeff + (EndAngleDiff / abs(EndAngleDiff)) * (1 - deaccellCoeff));

  lcd::set_text(5, "AngleDiff: " + to_string((int)(AngleDiff * 180 / M_PI)) + "EndDiff: " + to_string((int)(EndAngleDiff * 180 / M_PI)));
  // lcd::set_text(6, "Input: " + to_string((int)Strafe) + " / " + to_string((int)Forwards) + " / " + to_string((int)Turn));
  Set_Drive(Strafe, Forwards, Turn, 0);
}

void Mech_Drive::Set_Point_Drive(double speed, double direction, double distance, double turnSpeed, double accelSpeed, double deaccelSpeed, bool wallAlign, double criticalPoint, double criticalMultiplier, std::array<double, 4> endVelo)
{
  bool blocking = false;

  bool stopIfOver = false;
  std::array<double, 2> drive_convert = Convert(speed, direction);

  double travelledDistance = this->Get_Distance();

  double leftX = drive_convert[1];
  double leftY = drive_convert[0];
  double rightX = turnSpeed;

  if (previousVelo == unstartedArray())
  {
    previousVelo = {0, 0, 0, 0};
  }

  double accelSpeedDifference = sqrt((leftX - previousVelo[0]) * (leftX - previousVelo[0]) + (leftY - previousVelo[1]) * (leftY - previousVelo[1]));

  double speedDifference = sqrt((leftX - endVelo[0]) * (leftX - endVelo[0]) + (leftY - endVelo[1]) * (leftY - endVelo[1]));

  if (abs(this->Get_Speed()) < 30 && abs(travelledDistance) > criticalPoint && !blocking)
  {
    pros::lcd::set_text(4, "Stopped" + to_string((int)Get_Speed()) + "Distance" + to_string((int)travelledDistance));

    Stop();
    previousVelo = endVelo;
    _is_running = false;
    return;
  }

  if (std::abs(travelledDistance) > distance + criticalPoint && stopIfOver)
  {

    Stop();
    pros::lcd::set_text(4, "drive exceed" + to_string((int)travelledDistance) + ">" + to_string((int)distance));
    if (blocking)
    {
      return;
    }
    previousVelo = endVelo;

    _is_running = false;
  }

  if (std::abs(travelledDistance - distance) > (distance / 50 + 15) * criticalMultiplier)
  {
    pros::lcd::set_text(3, to_string((int)Get_Distance()) + "distance away " + to_string((int)abs(travelledDistance - distance)) + "critical" + to_string((int)distance / 50 + 15));

    // double accelCoeff = pow((abs(travelledDistance) + (distance/8)) / ((distance)) / 1, (1 / (2 * accelSpeed))) * abs(travelledDistance) / (travelledDistance) * 127;
    // double deaccelCoeff = pow(abs(travelledDistance - distance) / ((distance)) / 1, (1 / (2 * accelSpeed))) * abs(travelledDistance - distance) / (distance - travelledDistance) * 150;
    double accelCoeff = (pros::c::millis() - started) * 127 * accelSpeed / 1000;
    double deaccelCoeff = ((abs(travelledDistance - distance) * deaccelSpeed * 127) / (800));

    if (deaccelCoeff < speedDifference)
    {
      leftX = leftX / abs(speed) * deaccelCoeff + endVelo[0];
      leftY = leftY / abs(speed) * deaccelCoeff + endVelo[1];
    }
    if ((accelCoeff < accelSpeedDifference) && previousVelo != unstartedArray())
    {
      leftX = leftX / abs(speed) * accelCoeff + previousVelo[0];
      leftY = leftY / abs(speed) * accelCoeff + previousVelo[1];
    }

    if (deaccelCoeff < abs(rightX - endVelo[2]))
    {
      rightX = deaccelCoeff * getSignOf(rightX - endVelo[2]);
    }

    if (accelCoeff < abs(rightX - previousVelo[2]) && previousVelo != unstartedArray())
    {
      rightX = accelCoeff * getSignOf(rightX - previousVelo[2]);
    }
    if (wallAlign)
    {

      rightX += UltraAlign();
    }
    pros::lcd::set_text(5, "deaccel" + to_string(0) + "speedDiff" + to_string((int)speedDifference) + "accel" + to_string((int)accelCoeff));
    pros::lcd::set_text(6, "rightY" + to_string((int)leftY) + "endVelo" + to_string((int)endVelo[0]) + "previousVelo" + to_string((int)previousVelo[0]));
    // pros::lcd::set_text(6, "leftY" + to_string((int)leftY) + "endVelo" + to_string((int)endVelo[1]) + "previousVelo" + to_string((int)previousVelo[1]));

    Set_Drive(leftX, leftY, rightX, 0);
  }
  else
  {
    Set_Drive(endVelo[0], endVelo[1], endVelo[2], 0);
    previousVelo = endVelo;
    // pros::lcd::set_text(4, "Drive Stopped" + to_string(distance));
    _is_running = false;
  }
}

void Mech_Drive::Stop()
{
  left_front_motor.move(0);
  right_front_motor.move(0);
  right_back_motor.move(0);
  left_back_motor.move(0);
  _pid_inputs = {0, 0, 0, 0};
}

//Empty default constructor for blank factory arguments.
Mech_Drive::Mech_Drive()
{
}

void Mech_Drive::Create()
{
  std::array<double, 4>
      tempArray = {0, 0, 0, 0};
  initial_position = {0, 0, 0, 0};
  _left_front_motor_controller = new Motor_Controller(left_front_pid_values[0], left_front_pid_values[1], left_front_pid_values[2], &left_front_motor);
  _left_back_motor_controller = new Motor_Controller(left_back_pid_values[0], left_back_pid_values[1], left_back_pid_values[2], &left_back_motor);
  _right_front_motor_controller = new Motor_Controller(right_front_pid_values[0], right_front_pid_values[1], right_front_pid_values[2], &right_front_motor);
  _right_back_motor_controller = new Motor_Controller(left_front_pid_values[0], left_front_pid_values[1], left_front_pid_values[2], &right_back_motor);
  _master_pid = new Pid((master_drive_pid_values)[0], (master_drive_pid_values)[1], (master_drive_pid_values)[2]);
  _master_error_average = 0;
  _master_setpoint = 0;
  // _master_pid->Set_Error(2600);
  previousVelo = unstartedArray();
}