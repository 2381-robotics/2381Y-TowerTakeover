#include "main.h"
#include "pros/api_legacy.h"

#define ECHO 7
#define PING 8
// #define GYRO1 6
// #define GYRO2 5
#define ECHO2 5
#define PING2 6
void ultras()
{
    pros::ADIUltrasonic ultrasonic(ECHO, PING);
    pros::ADIUltrasonic ultrasonic2(ECHO2, PING2);
    pros::ADIAnalogIn gyro1(2);
    pros::ADIAnalogIn gyro2(3);
    gyro2.calibrate();
    gyro1.calibrate();
    int distance = 0;
    int distance2 = 0;
    int angle = 0;
    int angle2 = 0;

    while (1)
    {
        pros::delay(20);
        distance = ultrasonic.get_value();
        distance2 = ultrasonic2.get_value();

        // pros::lcd::set_text(1, std::to_string(gyro1.get_value()));
        // pros::lcd::set_text(2, std::to_string(gyro2.get_value()));
        pros::lcd::set_text(3, "Distance 1 " + std::to_string(distance));
        pros::lcd::set_text(4, "Distance 2 " + std::to_string(distance2));
        // pros::lcd::set_text(5, std::to_string(angle) + "<-- value, angle:  " + std::to_string(angle * 90 / 5819));
        // pros::lcd::set_text(6, std::to_string(angle2) + "<-- value, angle:  " + std::to_string(angle2 * 720 / 49033));
    }

    // pros::lcd::set_text(1, std::to_string(vision_indexer.get_object_count()));
    // pros::lcd::set_text(2, std::to_string(rtn.signature));
    // pros::lcd::set_text(3, std::to_string(rtn.angle));
    // pros::lcd::set_text(4, std::to_string(rtn.x_middle_coord));
    // pros::lcd::set_text(5, std::to_string(rtn.y_middle_coord));
    // pros::lcd::set_text(6, std::to_string(rtn.width));
    // pros::lcd::set_text(7, std::to_string(rtn.height));
}