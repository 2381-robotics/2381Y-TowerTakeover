#ifndef APP_HPP
#define APP_HPP
#include "component.hpp"
#include "api.h"
#include <vector>
#include <unordered_map>

class App : public Component_Base 
{
  public:
  lv_obj_t* createComponent(lv_obj_t* context);

  void render(lv_obj_t* parentContext);

  // using Component_Base::Component_Base;

  explicit App();

  protected: 
  static std::unordered_map<std::string, std::string> $initialState();
  
};

void screen(void);

#endif