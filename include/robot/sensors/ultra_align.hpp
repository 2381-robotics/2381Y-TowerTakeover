#ifndef ULTRA_FINDER_HPP
#define ULTRA_FINDER_HPP
#include <vector>
#include "api.h"
class UltraFinder
{
    public:
        UltraFinder(pros::ADIUltrasonic *left_ultra_, pros::ADIUltrasonic* right_ultra_);
        double prevLeft = 0, prevRight = 0;
        int numberTimes = 0;
        void Reset_Ultras();
        void Update_Angle();
        double Ultra_Angle();
        std::vector<double> leftDistances {}, rightDistances {}, masterDistances{};

        pros::ADIUltrasonic* left_ultra;
        pros::ADIUltrasonic* right_ultra;
};
#endif