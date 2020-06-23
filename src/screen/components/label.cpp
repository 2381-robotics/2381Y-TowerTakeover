
#include "api.h"
#include "label.hpp"
#include "utils.hpp"

#include <unordered_map>
#include <functional>
#include <vector>

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

using namespace std;

  lv_obj_t* Label::createComponent(lv_obj_t* context)
  {
    auto label =  lv_label_create(context, NULL);
    lv_label_set_text(label, textProp->second.c_str());
    print("label text: " + textProp->second);
    return label;
  }

  Label::Label(unordered_map<string, string>::iterator textRef, vector<Component_Base*> children) : Component_Base(children), textProp(textRef)
  {

  }
  Label::Label(string text, vector<Component_Base*> children) : Component_Base(children), text(text) { }