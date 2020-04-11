#ifndef _LABEL_HPP_
#define _LABEL_HPP_

#include "api.h"
#include "screen/component.hpp"
#include <unordered_map>
#include <vector>

class Label : public Component_Base 
{
  std::unordered_map<std::string, std::string>::iterator textProp;
  std::string text;

  public:

  lv_obj_t* createComponent(lv_obj_t* context);

  explicit Label( std::unordered_map<std::string, std::string>::iterator textRef, std::vector<Component_Base*> children = {});
  explicit Label(std::string text, std::vector<Component_Base*> children = {});

  using Component_Base::Component_Base;
};

#endif  //!_LABEL_HPP_