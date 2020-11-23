#ifndef __LIGHT_INDEXER__H__
#define __LIGHT_INDEXER__H__

#include "api.h"
#include <vector>

class Light_Indexer
{
    public: 
    void Update();
    bool initialized = false;
    double Get_Value(void);

    Light_Indexer(int indexer_port);
    void Create();

    protected: 
    int port;
    std::vector<double> RecentValues = {0,0,0};
    pros::ADIAnalogIn* light_sensor = nullptr;
};
#endif  //!__LIGHT_INDEXER__H__