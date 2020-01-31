#include "position_tracker.hpp"
#include "globals.hpp"
#include "api.h"
#include <array>
using namespace pros;
using namespace std;
Position_Tracker *Position_Tracker::instance() {
    static Position_Tracker instance;
    return &instance;
}

void Position_Tracker::Create() {
    left_encoder_ = &encoder_left;
    right_encoder_ = &encoder_right;
    back_encoder_ = &encoder_back;
}
// Position_Tracker::Position_Tracker() {}
void Position_Tracker::Set_Position(double XPos, double YPos, double Angle) 
{
    current_position = {XPos, YPos, Angle};
}

void Position_Tracker::Track_Position() 
{
    //global
    current_encoder_values[left_] = left_encoder_->get_value();
    current_encoder_values[right_] = right_encoder_->get_value();
    current_encoder_values[back_] = back_encoder_->get_value();

    position_change[left_] = (current_encoder_values[left_] - last_encoder_values[left_]) * PI * wheel_diameters[left_] / 360; // The amount the left side of the robot moved
    position_change[right_] = (current_encoder_values[right_] - last_encoder_values[right_]) * PI * wheel_diameters[right_] / 360;
    position_change[back_] = (current_encoder_values[back_] - last_encoder_values[back_]) * PI * wheel_diameters[back_] / 360;
    last_encoder_values = current_encoder_values;
    
    float hyp;                                                 //hypotenuse of triangle (starting center, end center, middle of circle it goes around)
    float i;                                                   //angle travelled * 0.5
    float h2;                                                  //same as hyp but using the back instead of the side wheels
    float angle = (position_change[left_] - position_change[right_]) / (wheel_offsets[left_] + wheel_offsets[right_]); //angle travelled
    if (angle != 0)
    {
        float radius = position_change[right_] / angle; // The radius of the circle the robot travel's around with the right side of the robot
        i = angle / 2.0;
        float sinI = sin(i);
        hyp = ((radius + wheel_offsets[right_]) * sinI) * 2.0;
        float r2 = position_change[back_] / angle; // The radius of the circle the robot travel's around with the back of the robot
        h2 = ((r2 + wheel_offsets[back_]) * sinI) * 2.0;
    }
    else
    {
        hyp = position_change[right_];
        i = 0;
        h2 = position_change[back_];
    }
    float p = i + current_position[WComp]; //ending angle
    float cosP = cos(p);
    float sinP = sin(p);

    //global position and angle update
    current_position[YComp] += hyp * cosP + h2*-sinP;
    current_position[XComp] += hyp * sinP +  h2 * cosP;
    current_position[WComp] += angle;

    //velocity tracking
    unsigned int currentTime = pros::millis();
    int passed = currentTime - velLastChecked;
    if (passed > 10)
    {
        current_velocity[WComp] = ((current_position[WComp] - last_position[WComp]) * 1000.0) / passed;
        current_velocity[XComp] = ((current_position[XComp] - last_position[XComp]) * 1000.0) / passed;
        current_velocity[YComp] = ((current_position[YComp] - last_position[YComp]) * 1000.0) / passed;


        last_position = current_position;
        velLastChecked = currentTime;
    }

    // lcd::set_text(1, "Current Pos: X-" + to_string((float)current_position[0])+ "Y-" + to_string((float)current_position[1])+ "W" + to_string((float)current_position[2]));
}  

array<double,3> Position_Tracker::Get_Position()
{
    return current_position;
}
array<double,3> Position_Tracker::Get_Velocity() 
{
    return current_velocity;
}
// Position_Tracker::Poistion_Tra  