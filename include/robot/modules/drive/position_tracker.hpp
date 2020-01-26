#ifndef POSITION_TRACKER_HPP
#define POSITION_TRACKER_HPP

#include "api.h"
#include <array>
#include <map>

class Position_Tracker {
    public:
    enum Tracking_Encoder {
        left_ = 0,
        right_, 
        back_
    };

    enum Vector_Component {
        XComp = 0,
        YComp,
        WComp,
    };
    static Position_Tracker* instance();
    void Track_Position();

    std::array<double,3> Get_Position();
    std::array<double,3> Get_Velocity();

    void Set_Position(double XPos, double YPos, double Angle);
    void Create();

    protected:

    pros::ADIEncoder* left_encoder_ = nullptr;
    pros::ADIEncoder *right_encoder_ = nullptr;
    pros::ADIEncoder * back_encoder_ = nullptr;
    std::array<double, 3> current_position = {0,0,0}, current_velocity = {0,0,0}, last_position = {0,0,0};
    std::array<double,3> current_encoder_values = {0,0,0}, last_encoder_values = {0,0,0}, position_change = {0,0,0};
    unsigned int velLastChecked = 0;
};


#endif