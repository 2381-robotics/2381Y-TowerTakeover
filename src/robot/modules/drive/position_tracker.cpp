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
    
    inertial_->reset();
}
// Position_Tracker::Position_Tracker() {}
void Position_Tracker::Set_Position(double XPos, double YPos, double Angle) 
{
    current_position = {XPos, YPos, Angle};
}


int calibrationStart = 0;


double Position_Tracker::Get_Angle()
{
    if(inertial_->is_calibrating()) return 0;

    auto angle = inertial_->get_yaw();
    auto corrected = angle;
    auto radians = -corrected * M_PI / 180;
    return radians;
}
void Position_Tracker::Track_Position() 
{
    if(inertial_->is_calibrating()) 
    {
        lcd::set_text(2,"IMU CALIBRATING");
        return;
    }
    auto quat = inertial_->get_euler();

//     return;
    angular_ori = { - quat.pitch * M_PI / 180, - quat.roll * M_PI/180, this->Get_Angle()};
    angular_vel = {angular_ori[0] - angular_last[0], angular_ori[1] - angular_last[1], fmod(angular_ori[2] - angular_last[2], 2*M_1_PI)};
    
    auto quatText = " X " + to_string(angular_ori[0]) + " Y " + to_string(angular_ori[1]);
    auto quatText2 = " z " + to_string(angular_ori[2]);


    // lcd::set_text(3, quatText);
    // lcd::set_text(4, quatText2);
    //global
    // current_encoder_values[left_] = left_encoder_->get_value();
    current_encoder_values[right_] = v_enc_->get_value();
    current_encoder_values[back_] = h_enc_->get_value();
    // lcd::set_text(2, "VERT: " + to_string(current_encoder_values[right_]) + " Horz: " + to_string(current_encoder_values[back_]));


    // position_change[left_] = (current_encoder_values[left_] - last_encoder_values[left_]) * PI * wheel_diameters[left_] / 360; // The amount the left side of the robot moved
    position_change[right_] = (current_encoder_values[right_] - last_encoder_values[right_]) * PI * wheel_diameters[right_] / 360;
    position_change[back_] = (current_encoder_values[back_] - last_encoder_values[back_]) * PI * wheel_diameters[back_] / 360;

    // Current Orientation - Angular Change/2 to get average between current and last angle measured.
    vert_velocity = (position_change[right_] - wheel_offsets[right_] * angular_vel[2]) * exp<double>(1i * (angular_ori[2] + M_PI/2 - angular_vel[2]/2));
    horz_velocity = (position_change[back_] - wheel_offsets[back_] * angular_vel[2]) * exp<double>(1i * (angular_ori[2] - angular_vel[2]/2));

    

    h_pos += horz_velocity;
    v_pos += vert_velocity;

    velocity = vert_velocity + horz_velocity;
    position += velocity;
    // lcd::set_text(5, "V POS " + to_string(current_encoder_values[right_]));
    // lcd::set_text(6, "H POS " + to_string(current_encoder_values[back_]));

    // lcd::set_text(5, "V POS : (" + to_string(v_pos.real()) + ", " + to_string(v_pos.imag()) + ")");
    // lcd::set_text(6, "H POS : (" + to_string(h_pos.real()) + ", " + to_string(h_pos.imag()) + ")");
    lcd::set_text(2, "POSITION : ("  + to_string((int)round( 100* position.real())) + ", " + to_string((int)round( 100* position.imag())) + "), " + to_string(round(angular_ori[2] * 180 / M_PI)) +" deg");
    angular_last = angular_ori;
    last_encoder_values = current_encoder_values;
        
}  

complex<double> Position_Tracker::Get_Position()
{
    return position;
}
array<double,3> Position_Tracker::Get_Velocity() 
{
    return current_velocity;
}
// Position_Tracker::Poistion_Tra  