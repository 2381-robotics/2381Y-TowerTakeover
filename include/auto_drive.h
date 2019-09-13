#ifndef AUTO_DRIVE_H
#define AUTO_DRIVE_H
#include "utils/robot/drive/drive.hpp"

#include <array>
#include <tuple>

class Auto_Drive{
public:
  Auto_Drive(Drive* drive);

  std::tuple<double, double> Convert(double speed = 0, double direction = 0);
  Drive* drive;
  void volatile  Set_Point_Drive(double *speed = 0, double *direction = 0, double *distance = 0);
private:
  double* _speed;
  double* _direction;
  double* _distance;

};
#endif
