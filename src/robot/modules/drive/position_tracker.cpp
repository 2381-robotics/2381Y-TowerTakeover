#include "position_tracker.hpp"
#include "globals.hpp"
#include "api.h"
#include <array>
#include <complex>
#include <cmath>

using namespace pros;
using namespace std;
using namespace std::complex_literals;

const complex<double> Position_Tracker::wheel_center_offset = {2.75, 2.25};

Position_Tracker *Position_Tracker::instance() {
    static Position_Tracker instance;
    return &instance;
}

void Position_Tracker::Create() {
    
    inertial_ = &imu;
    v_enc_ = &v_enc;
    h_enc_ = &h_enc;

    Reset();
    // Maybe take an input or global on initial position;
    Set_Position(0,0);
    // Maybe take an input or global on initial position;
    // Set_Position(0, 0);
}

void Position_Tracker::Reset()
{
    inertial_->reset();
    v_enc_->reset();
    h_enc_->reset();
    current_encoder_values = last_encoder_values = position_change = {0,0,0};
    ang_disp = ang_last = ang_vel = ang_origin = 0;
    h_disp = v_disp = h_vel = v_vel = origin = v_disp_n = h_disp_n = 0;
}


const void Position_Tracker::Set_Position(complex<double> position_, double angle_, complex<double> oldPos, double oldAngle) 
{
    origin = position_ - oldPos + origin;

    ang_origin = angle_ - oldAngle + ang_origin;
    ang_last = Get_Angle();
}


int calibrationStart = 0;


double Position_Tracker::Get_Angle()
{
    if(inertial_->is_calibrating()) return 0;

    auto angle = inertial_->get_yaw();
    auto corrected = angle;
    // Corrected is adjusted for drift?

    auto radians = -corrected * M_PI / 180;
    return remainder(radians + ang_origin, 2 * M_PI);
}

void Position_Tracker::Track_Position() 
{
    if(inertial_->is_calibrating()) 
    {
        lcd::set_text(2,"IMU CALIBRATING");
        return;
    }

    // Current Angle, Angular Dispertion. This is normalized to between (-M_PI, M_PI).
    ang_disp = this->Get_Angle();

    // Angular Velocity 
    ang_vel = fmod(ang_disp - ang_last, 2*M_PI);
    auto normalized_ang_vel = NormalizeAngle(ang_disp - ang_last);

    current_encoder_values[right_] = v_enc_->get_value();
    current_encoder_values[back_] = h_enc_->get_value();
    // lcd::set_text(2, "VERT: " + to_string(current_encoder_values[right_]) + " Horz: " + to_string(current_encoder_values[back_]));

    // Position change is the swept angle multiplied by the radius. Radius = 1/2 Diameter, so it is \Delta_Angle * M_PI/180 *Diameter/2.
    position_change[right_] = (current_encoder_values[right_] - last_encoder_values[right_]) * PI * wheel_diameters[right_] / 360;
    position_change[back_] = (current_encoder_values[back_] - last_encoder_values[back_]) * PI * wheel_diameters[back_] / 360;

    // Current Orientation - Angular Change/2 to get average between current and last angle measured. 
    // Angular velocity is normalized to (-M_PI, M_PI) so that the change in angle always calculates as the closest side.
    // Otherwise the vector will be pointing in the opposite direction.
    
    v_vel = (position_change[right_]) * exp<double>(1i * (ang_disp - ang_vel * 1/2));
    h_vel = ( position_change[back_]) * exp<double>(1i * (ang_disp - M_PI/2 - ang_vel * 1/2));    

    v_vel_n = (position_change[right_]) * exp<double>(1i * (ang_disp - normalized_ang_vel * 1/2));
    h_vel_n = (position_change[back_]) * exp<double>(1i * (ang_disp - M_PI/2 - normalized_ang_vel * 1/2));

    v_disp_n += v_vel_n;
    h_disp_n += h_vel_n;

    h_disp += h_vel;
    v_disp += v_vel;

    ang_last = ang_disp;
    last_encoder_values = current_encoder_values;
    
    lcd::set_text(1, "POS DIFF : ("  + to_string((int)round( 100* (Get_Position().real() - Get_Position_N().real()))) + ", " + to_string((int)round( 100* (Get_Position().imag() - Get_Position_N().imag()))) + "), " + to_string(round(ang_disp * 180 / M_PI)) +" deg");
    lcd::set_text(2, "N_POS : ("  + to_string((int)round( 100* Get_Position_N().real())) + ", " + to_string((int)round( 100* Get_Position_N().imag())) + "), " + to_string(round(ang_disp * 180 / M_PI)) +" deg");
}  

complex<double> Position_Tracker::Get_Position()
{
    auto initial_wheel_displacement = Position_Tracker::wheel_center_offset * exp<double>(1i * ang_origin);
    auto wheel_displacement = Position_Tracker::wheel_center_offset * exp<double>(1i * Get_Angle());
    return Get_Displacement() + origin - initial_wheel_displacement + wheel_displacement;
}
complex<double> Position_Tracker::Get_Position_N()
{
    auto initial_wheel_displacement = Position_Tracker::wheel_center_offset * exp<double>(1i * ang_origin);
    auto wheel_displacement = Position_Tracker::wheel_center_offset * exp<double>(1i * Get_Angle());
    return v_disp_n + h_disp_n + origin - initial_wheel_displacement + wheel_displacement;
}

complex<double> Position_Tracker::Get_Displacement()
{
    return v_disp + h_disp;
}

complex<double> Position_Tracker::Get_Velocity() 
{
    return v_vel + h_vel;
}
// Position_Tracker::Poistion_Tra  