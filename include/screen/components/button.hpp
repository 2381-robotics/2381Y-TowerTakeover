#ifndef __BUTTON__H__
#define __BUTTON__H__

#include "screen/component.hpp"
#include "api.h"
#include <functional>
#include <unordered_map>
#include <vector>

class Button : public Component_Base 
{
  public: 
  explicit Button(std::vector<Component_Base*> children = {}, std::function<void(void)> onClick = [](void) -> void {});
  
  using Component_Base::Component_Base;

  lv_obj_t* createComponent(lv_obj_t* context);
  void handleClick();

    static lv_res_t button_click(lv_obj_t * btn);

  protected: 
  std::function<void(void)> onClick;
};

 
#endif  //!__BUTTON__H__