#ifndef LIFT_HPP
#define LIFT_HPP

#include "robot/structure.hpp"
class Lift : public Structure{
public:
   Lift();
  virtual void Set_Target(double target_height) = 0;
  virtual double Get_Height() = 0;
  virtual double Get_Target() = 0;
  virtual void Move_Lift() = 0;
  virtual void Increment_Height(int increment) = 0;
  virtual void Smooth_Lift(int increment) = 0;
  void Reset() override;
  std::string name = "Lift";

protected: 

  double _lift_height, _lift_power, _lift_speed;
  double _target_height;
  double _min_height, _max_height;
  
};


#endif
 