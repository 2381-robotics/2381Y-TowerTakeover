#ifndef DRIVE_H
#define DRIVE_H
class Drive {
public:
  Drive();
  virtual void Set_Drive(double left_x, double left_y, double right_x, double right_y);
  virtual double Get_Speed();

private:
};


#endif
