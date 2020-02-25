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
  double ultra = 1.5 * (abs(rightAverage - leftAverage) / (rightAverage - leftAverage) * 60 * pow(abs(rightAverage - leftAverage) / 60, 0.5));
  masterDistances.push_back(ultra);
  if(masterDistances.size() > 2)
  {
    masterDistances.erase(masterDistances.begin());
  }
}
double UltraFinder::Ultra_Angle()
{

  double leftDistance = leftDistances[0], rightDistance = rightDistances[0];
  
  

    // prevLeft = leftDistance;
    // prevRight = rightDistance;


  double masterAverage = 0;
  for(auto it =  masterDistances.begin(); it != masterDistances.end(); it++)
  {
    masterAverage += *it; 
  }
  if(masterDistances.size() > 0)
  {
    masterAverage = masterAverage / masterDistances.size();
  }

  
  lcd::set_text(1, to_string(leftDistance) + "DIE" + to_string(rightDistance) + " " + to_string((int)masterAverage));
    return masterAverage;
}