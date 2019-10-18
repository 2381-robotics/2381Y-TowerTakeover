#ifndef LIFT_HPP
#define LIFT_HPP

#include "utils/robot/structure.hpp"
class Lift : public Structure{
public:
  Lift();
  virtual void Set_Height(double target_height) = 0;
  virtual double Get_Height() = 0;
  virtual double GetTarget() = 0;

protected: 
  double _lift_height;
  double _target_height;
  double _lift_difference;
  
};


#endif
 