#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <vector>
#include "main.h"
#include <functional>
class Component {
    public:

    Component(Component* parent) : parent(parent)
    {
        
    };

    static std::function<Component*(Component*)> yeet()
    {
        return [&](Component* parent) -> Component* 
        {

            return new Component(parent);
        };
    };

    lv_obj_t* component;

    

    Component* parent;

};

#endif




new Component::StaticMethod(
    
)

