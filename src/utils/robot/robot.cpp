#include "robot.hpp"
#include "../../globals.hpp"
#include "robot_factory.hpp"
#include "api.h"
Robot::~Robot() {}
void Robot::set_drive(double left_x, double left_y, double right_x, double right_y) {
  pros::lcd::set_text(7, "Yeet:");
  this->drive->Set_Drive(left_x, left_y, right_x, right_y);
}
void Robot::create(){
  this->drive->create();
}
Robot::Robot(){}

void Robot::assign_drive(Drive* drive_instance) {
  this->drive = drive_instance;
}
void Robot::assign_lift(Lift* lift_instance) {
  this->lift = lift_instance;
}
void Robot::assign_intake(Structure* intake_instance) {
  this->intake = intake_instance;
}

Robot* robot = RobotFactory::instance()->create_robot("Mech_Drive","Y_Lift","");
