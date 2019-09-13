#include "robot.hpp"
#include "../../globals.hpp"
#include "robot_factory.hpp"

Robot::~Robot() {}
void Robot::set_drive(double left_x, double left_y, double right_x, double right_y) {
  this->drive->Set_Drive(left_x, left_y, right_x, right_y);
}
void Robot::create(){}
Robot::Robot(){}
Robot* robot = RobotFactory::instance()->create_robot("Mech_Drive","","");

void Robot::assign_drive(Drive* drive_instance) {
  this->drive = drive_instance;
}
void Robot::assign_lift(Structure* drive_instance) {
  this->lift = drive_instance;
}
void Robot::assign_intake(Structure* drive_instance) {
  this->intake = drive_instance;
}
