
#ifndef MODULE_HPP_
#define MODULE_HPP_
#include <map>
#include <vector>
#include "structure.hpp"




class Module : public Structure
{
public:
    /**
   		* Constructor
   * Abstract Parent Class, is inherited by structural objects
   */
    virtual void Stop() = 0;
    void Run();
    // virtual void Debug() = 0;

protected:
    virtual void Move_Motor() = 0;
};

#endif
