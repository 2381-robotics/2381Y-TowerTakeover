#ifndef _TAB_HPP
#define _TAB_HPP

#include "api.h"
#include "screen/component.hpp"
#include <unordered_map>
#include <vector>
#include <map>

class Tab : public Component_Base 
{
  std::map<std::string, Component_Base*> tabs;
    
  public:

  lv_obj_t* createComponent(lv_obj_t* context);
    std::string name;
    
  explicit Tab(std::string name, std::vector<Component_Base*> children = {});

  using Component_Base::Component_Base;
};

#endif  //!_LABEL_HPP_