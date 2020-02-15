#ifndef ULTRA_FINDER_HPP
#define ULTRA_FINDER_HPP
#include "api.h"
class UltraFinder
{
    public:
        UltraFinder(pros::ADIUltrasonic *left_ultra_, pros::ADIUltrasonic* right_ultra_);
        double prevLeft = 0, prevRight = 0;
        int numberTimes = 0;
        void Reset_Ultras();
        double Ultra_Angle();
        pros::ADIUltrasonic* left_ultra;
        pros::ADIUltrasonic* right_ultra;
};
#endif