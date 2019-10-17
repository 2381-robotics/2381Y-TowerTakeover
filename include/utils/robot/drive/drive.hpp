#ifndef DRIVE_H
#define DRIVE_H

#include "utils/robot/structure.hpp"
class Drive : public Structure{
public:
  Drive();
  virtual void Set_Drive(double left_x, double left_y, double right_x, double right_y) = 0;
  virtual double Get_Speed() = 0;


};


#endif
