#include "ultra_align.hpp"
#include "api.h"

using namespace pros;
using namespace std;

UltraFinder::UltraFinder(ADIUltrasonic* left, ADIUltrasonic* right):  left_ultra(left), right_ultra(right)
{

};
void UltraFinder::Reset_Ultras()
{
    prevLeft = left_ultra->get_value();
    prevRight = right_ultra->get_value();
    numberTimes = 0;
}
double UltraFinder::Ultra_Angle()
{
  double leftDistance = left_ultra->get_value();
  double rightDistance = right_ultra->get_value();

      prevLeft = (prevLeft*2 + leftDistance)/3;
      prevRight = (prevRight * 2 + rightDistance) / 3;

    // prevLeft = leftDistance;
    // prevRight = rightDistance;
  if(prevLeft == prevRight)
  {
      return 0;

  }

  lcd::set_text(0, to_string(prevRight) + "DIE" + to_string(prevLeft));
  double ultra =  1.5 * (abs(prevRight - prevLeft) / (prevRight - prevLeft) * 60 * pow(abs(prevRight - prevLeft) / 60, 0.5));
    return ultra;
}