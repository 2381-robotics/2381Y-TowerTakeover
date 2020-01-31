#include "robot.hpp"
#include "globals.hpp"
#include "robot_factory.hpp"
#include "api.h"
#include <map>

Robot::~Robot() {}

void Robot::set_drive(double left_x, double left_y, double right_x, double right_y) {
  this->drive->Set_Drive(left_x, left_y, right_x, right_y);
}
void Robot::set_point_drive(double speed, double direction, double distance, double turnSpeed, double accelScaling, bool blocking, double criticalPoint, double criticalMultiplier) {
  this->drive->Set_Point_Drive(speed, direction, distance, turnSpeed, accelScaling, blocking, criticalPoint, criticalMultiplier);
}

Robot::Robot(){}

void Robot::assign_drive(Mech_Drive* drive_instance) {
  this->drive = drive_instance;
}
void Robot::assign_lift(Lift* lift_instance) {
  this->lift = lift_instance;
}
void Robot::assign_intake(Structure* intake_instance) {
  this->intake = intake_instance;
}
void Robot::Create() {

}
Robot* robot = RobotFactory::instance()->create_robot("Mech_Drive","Y_Lift","");



void Robot::Reset(){
}
// void Robot::Debug() {}
void Robot::stop() {
  this->drive->Stop();
}