#include "position_tracker.hpp"
#include "globals.hpp"
#include "api.h"
#include <array>
#include <complex>
#include <cmath>

using namespace pros;
using namespace std;
using namespace std::complex_literals;

Position_Tracker *Position_Tracker::instance() {
    static Position_Tracker instance;
    return &instance;
}

void Position_Tracker::Create() {
    
    inertial_ = &imu;
    v_enc_ = &v_enc;
    h_enc_ = &h_enc;

    // Maybe take an input or global on initial position;
    Set_Position(0, M_PI/2);
}

void Position_Tracker::Reset()
{
    inertial_->reset();
    v_enc_->reset();
    h_enc_->reset();
    current_encoder_values = last_encoder_values = position_change = {0,0,0};
    ang_disp = ang_last = ang_vel = ang_origin = 0;
    h_disp = v_disp = h_vel = v_vel = origin = 0;
}

void Position_Tracker::Set_Position(complex<double> position_, double angle_) 
{
    Reset();
    origin = position_;
    ang_origin = ang_last = angle_;
}


int calibrationStart = 0;


double Position_Tracker::Get_Angle()
{
    if(inertial_->is_calibrating()) return 0;

    auto angle = inertial_->get_yaw();
    auto corrected = angle;
    // Corrected is adjusted for drift?

    auto radians = -corrected * M_PI / 180;
    return fmod(radians + ang_origin, 2 * M_PI);
}

void Position_Tracker::Track_Position() 
{
    // if(inertial_->is_calibrating()) 
    // {
    //     lcd::set_text(2,"IMU CALIBRATING");
    //     return;
    // }
    auto quat = inertial_->get_euler();

    ang_disp = this->Get_Angle();
    ang_vel = fmod(ang_disp - ang_last, 2*M_1_PI);
    
    auto quatText = " X " + to_string(ang_disp) + " Y " + to_string(ang_disp);
    auto quatText2 = " z " + to_string(ang_disp);

    // lcd::set_text(3, quatText);
    // lcd::set_text(4, quatText2);

    current_encoder_values[right_] = v_enc_->get_value();
    current_encoder_values[back_] = h_enc_->get_value();
    // lcd::set_text(2, "VERT: " + to_string(current_encoder_values[right_]) + " Horz: " + to_string(current_encoder_values[back_]));

    position_change[right_] = (current_encoder_values[right_] - last_encoder_values[right_]) * PI * wheel_diameters[right_] / 360;
    position_change[back_] = (current_encoder_values[back_] - last_encoder_values[back_]) * PI * wheel_diameters[back_] / 360;

    // Current Orientation - Angular Change/2 to get average between current and last angle measured.
    v_vel = (position_change[right_] - wheel_offsets[right_] * ang_vel) * exp<double>(1i * (ang_disp - ang_vel/2));
    // h_vel = 0;
    h_vel = ( 1.75 * position_change[back_] - wheel_offsets[back_] * ang_vel) * exp<double>(1i * (ang_disp - + M_PI/2 - ang_vel/2));    

    h_disp += h_vel;
    v_disp += v_vel;

    lcd::set_text(5, "V POS " + to_string(current_encoder_values[right_]));
    lcd::set_text(6, "H POS " + to_string(current_encoder_values[back_]));

    // lcd::set_text(5, "V POS : (" + to_string(v_disp.real()) + ", " + to_string(v_disp.imag()) + ")");
    // lcd::set_text(6, "H POS : (" + to_string(h_disp.real()) + ", " + to_string(h_disp.imag()) + ")");
    
    lcd::set_text(2, "POSITION : ("  + to_string((int)round( 100* Get_Position().real())) + ", " + to_string((int)round( 100* Get_Position().imag())) + "), " + to_string(round(ang_disp * 180 / M_PI)) +" deg");
    ang_last = ang_disp;
    last_encoder_values = current_encoder_values;
}  

complex<double> Position_Tracker::Get_Position()
{
    return Get_Displacement() + origin;
}

complex<double> Position_Tracker::Get_Displacement()
{
    return v_disp;
}

complex<double> Position_Tracker::Get_Velocity() 
{
    return v_vel + h_vel;
}
// Position_Tracker::Poistion_Tra  