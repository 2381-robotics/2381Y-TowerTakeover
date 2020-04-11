#include "screen/app.hpp"
#include "api.h"
#include "button.hpp"
#include "label.hpp"
#include "utils.hpp"

#include <unordered_map>
#include <functional>
#include <vector>

using namespace std;

  Button::Button(vector<Component_Base*> children, function<void(void)> onClick) : Component_Base(children), onClick(onClick) {

  }
  

  lv_obj_t* Button::createComponent(lv_obj_t* context)
  {
    auto button = lv_btn_create(context, NULL);
    // lv_obj_set_free_ptr(button, this);
    lv_btn_set_action(button, LV_BTN_ACTION_CLICK, button_click);
    lv_obj_set_style(button, &lv_style_pretty_color);
    return button;
  }
  void Button::handleClick()
  {
    this->onClick();
  }


lv_res_t Button::button_click(lv_obj_t * btn)
{
  print("buttonClickfn");
  Button* button_node = static_cast<Button*>(btn->free_ptr);
  button_node->handleClick();
  return LV_RES_OK;
}
 