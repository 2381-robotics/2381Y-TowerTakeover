#ifndef DRIVE_HPP
#define DRIVE_HPP

#include "utils/robot/structure.hpp"
class Drive : public Structure{
public:
  Drive();
  virtual void Set_Drive(double left_x, double left_y, double right_x, double right_y) = 0;
  virtual void Set_Point_Drive(double speed, double direction, double distance) = 0;
  virtual double Get_Speed() = 0;
  virtual double Get_Distance() = 0;
  virtual void Reset_Point() = 0;
  virtual void Set_Init_Point()= 0;
  virtual bool get_running() = 0;
  virtual void Set_Turn(double Speed, double direction, double distance) = 0;
  // ~Drive();
  // virtual void Pid_Set_Point_Drive();

  // private:

};


#endif
