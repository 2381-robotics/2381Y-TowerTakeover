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
}
void Light_Indexer::Update()
{
    if(!isInitialized) return;
    auto sensorValue = light_sensor->get_value();
    RecentValues.insert(RecentValues.begin(), sensorValue);
    RecentValues.resize(3);
}
double Light_Indexer::Get_Value(void)
{
    if(!isInitialized) return 0;
    return std::accumulate(RecentValues.begin(), RecentValues.end(), 0)/RecentValues.size();
}

void Light_Indexer::Reset(void)
{
    RecentValues = {0,0,0};
    delete light_sensor;
    isInitialized = false;
}