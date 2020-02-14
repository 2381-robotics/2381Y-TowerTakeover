#include "api.h"
#include "vision_indexer.hpp"
using namespace pros;
using namespace std;
VisionIndexer::VisionIndexer(Vision* sensor_) : vision_sensor(sensor_)
{

};

void VisionIndexer::Set_Sig(int slot, vision_signature_s_t sig)
{
    vision_sensor->set_signature(slot, &sig);
}

int VisionIndexer::Check_Object() const {
    return ((vision_sensor->get_object_count() > 0) && (Get_Object().height > 20));
}


vision_object_s_t VisionIndexer::Get_Object() const
{
    return vision_sensor->get_by_size(0);
}