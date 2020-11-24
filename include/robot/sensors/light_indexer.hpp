#ifndef __LIGHT_INDEXER__H__
#define __LIGHT_INDEXER__H__

#include "api.h"
#include <vector>
#include "robot/structure.hpp"

class Light_Indexer : public Structure
{
    public: 
    void Update();
    double Get_Value(void);

    Light_Indexer(int indexer_port);
    void Reset(void) override;
    
    protected: 
    void Create() override;
    int port;
    std::vector<double> RecentValues = {0,0,0};
    pros::ADIAnalogIn* light_sensor = nullptr;
};
#endif  //!__LIGHT_INDEXER__H__