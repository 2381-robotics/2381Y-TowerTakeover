#include "ultra_align.hpp"
#include "api.h"

using namespace pros;
using namespace std;

UltraFinder::UltraFinder(ADIUltrasonic* left, ADIUltrasonic* right):  left_ultra(left), right_ultra(right)
{

};
void UltraFinder::Reset_Ultras()
{
    leftDistances.resize(0);
    rightDistances.resize(0);
}

void UltraFinder::Update_Angle()
{
  double leftDistance = left_ultra->get_value();
  double rightDistance = right_ultra->get_value();
  leftDistances.insert(leftDistances.begin(), leftDistance);
  rightDistances.insert(rightDistances.begin(), rightDistance);
  if(leftDistances.size() >15)
  {
    leftDistances.pop_back();
    rightDistances.pop_back();
  }
  double leftAverage = 0, rightAverage = 0;
  for (int i = 0; i < leftDistances.size(); i++)
  {
    leftAverage += leftDistances[i];
    rightAverage += rightDistances[i];
  }
  if (leftDistances.size() != 0)
  {
    leftAverage = leftAverage / leftDistances.size();
    rightAverage = rightAverage / rightDistances.size();
  }
  double ultra = 1.4 * (abs(rightAverage - leftAverage) / (rightAverage - leftAverage) * 60 * pow(abs(rightAverage - leftAverage) / 60, 0.5));
  masterDistances.push_back(ultra);
  if(masterDistances.size() >2)
  {
    masterDistances.erase(masterDistances.begin());
  }
}
double previousAlign1;
double UltraFinder::Ultra_Angle()
{

   double leftDistance = left_ultra->get_value();
  double rightDistance = right_ultra->get_value();
  if(leftDistance == rightDistance){
    return 0;
  }
  lcd::set_text(0, to_string(rightDistance) + "DIE" + to_string(leftDistance));
  double ultra =  1.5 * (abs(rightDistance - leftDistance) / (rightDistance - leftDistance) * 70 * pow(abs(rightDistance - leftDistance) / 70, 0.5));
  if(ultra != NAN && abs(ultra) < 500)
  {
    previousAlign1 = (ultra * 2 + previousAlign1)/3;
  }
  return previousAlign1;
}