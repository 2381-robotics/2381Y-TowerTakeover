#include "screen/component.hpp"
#include "api.h"
#include "utils.hpp"
#include <vector>
using namespace std;

Component_Base::Component_Base(vector<Component_Base*> children) : $state($initialState()) {
    assignChildren(children);
}
  
Component_Base* Component_Base::create() {};

const string Component_Base::$getState(string key)
{
   return this->$state[key];
}

void Component_Base::$setState(string key, string value)
{
    print("Set State");
    print((key + " " + value));


    if(this->$state.find(value)!= $state.end())
    {
      // $state.insert(make_pair("key", "value"));
      print("Key Not Found");


    } else{
      this->$state[key] = value;
      print("Key Found" + $state[key]);
    }
    render(parentContext);
}

Component_Base::Component_Base()  {
$state = this->$initialState();
}

unordered_map<string, string> Component_Base::$initialState() 
{
    return {};
}

  void Component_Base::render(lv_obj_t* newContext) {
    // print("rerendering something");
    parentContext = newContext;
    screen = createComponent(parentContext);
    screen->free_ptr = this;
    for(auto i : children){
      i->render(screen);
    };
  }

  void Component_Base::assignChildren(vector<Component_Base*> children) {
    this->children = children;
  }

  void Component_Base::Render(Component_Base* i, lv_obj_t* screen)
  {
    i->render(screen);
  }
  