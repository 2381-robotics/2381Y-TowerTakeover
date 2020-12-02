#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "main.h"
#include <vector>
#include <unordered_map>

class Component_Base
{
  public: 

  lv_obj_t* screen;
  
  std::vector<Component_Base*> children;

  explicit Component_Base(std::vector<Component_Base*> children);
  
  // static Component_Base* create();

  std::unordered_map<std::string, std::string> $state;

  const std::string $getState(std::string key);

  void $setState(std::string key, std::string value);

  virtual lv_obj_t* createComponent(lv_obj_t* context) = 0;

  protected :

  Component_Base();

  lv_obj_t* parentContext;

  static std::unordered_map<std::string, std::string> $initialState();

  void render(lv_obj_t* newContext);

  void assignChildren(std::vector<Component_Base*> children = {});

  static void Render(Component_Base* i, lv_obj_t* screen);
};

#endif