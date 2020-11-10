#ifndef _TAB_VIEW_HPP
#define _TAB_VIEW_HPP

#include "api.h"
#include "screen/component.hpp"
#include <unordered_map>
#include <vector>
#include <map>
#include "tab.hpp"

class TabView : public Component_Base 
{
  std::map<std::string, Component_Base*> tabs;
    
  public:

  lv_obj_t* createComponent(lv_obj_t* context);

  using Component_Base::Component_Base;

};

#endif  //!_LABEL_HPP_