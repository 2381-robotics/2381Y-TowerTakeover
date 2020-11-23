#include "light_indexer.hpp"
#include "api.h"
#include <numeric>

Light_Indexer::Light_Indexer(int port) : port(port)
{

}

void Light_Indexer::Create()
{
    light_sensor = new pros::ADIAnalogIn(port);
    light_sensor->calibrate();
    initialized = true;
}
void Light_Indexer::Update()
{
    if(!initialized) return;
    auto sensorValue = light_sensor->get_value();
    RecentValues.insert(RecentValues.begin(), sensorValue);
    RecentValues.resize(3);
}
double Light_Indexer::Get_Value(void)
{
    if(!initialized) return 0;
    return std::accumulate(RecentValues.begin(), RecentValues.end(), 0)/RecentValues.size();
}
