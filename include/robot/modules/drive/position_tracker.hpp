#ifndef POSITION_TRACKER_HPP
#define POSITION_TRACKER_HPP

#include "api.h"
#include <array>
#include <map>
#include <complex>

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

    std::complex<double> Get_Position();
    std::complex<double> Get_Displacement();

    std::complex<double> Get_Velocity();

    double Get_Angle();

    void Set_Position(std::complex<double> position = 0, double Angle = 0);
    void Reset();

    void Create();

    protected:

//Vertical & Horizontal Encoder

    pros::ADIEncoder *v_enc_ = nullptr;
    pros::ADIEncoder * h_enc_ = nullptr;
    pros::Imu* inertial_ = nullptr;


    double ang_disp = 0, ang_vel = 0, ang_last = 0, ang_origin = 0;

    std::complex<double> origin = 0;

    std::complex<double> h_disp = 0, v_disp = 0;

    std::complex<double> v_vel = 0, h_vel = 0;

    std::array<double, 3> current_position = {0,0,0}, current_velocity = {0,0,0}, last_position = {0,0,0};
    std::array<double,3> current_encoder_values = {0,0,0}, last_encoder_values = {0,0,0}, position_change = {0,0,0};
    unsigned int velLastChecked = 0;
};


#endif