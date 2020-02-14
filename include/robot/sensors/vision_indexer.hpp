#ifndef VISION_INDEXER_HPP
#define VISION_INDEXER_HPP
#include "api.h"
class VisionIndexer
{
    public:
    VisionIndexer(pros::Vision * sensor_);
    void Set_Sig(int slot, pros::vision_signature_s_t sig);

    int Check_Object() const;
    pros::vision_object_s_t Get_Object() const;

    pros::Vision *vision_sensor;
};


#endif