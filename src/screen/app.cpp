#include "screen/app.hpp"
#include "api.h"
#include "components/button.hpp"
#include "components/label.hpp"
#include "utils.hpp"
#include "components/tab.hpp"
#include "components/tabview.hpp"

#include <unordered_map>
using namespace std;

  lv_obj_t* App::createComponent(lv_obj_t* context)
  {
    return lv_scr_act();
  }

  void App::render(lv_obj_t* parentContext) {
    print("Rerender App");
    screen = createComponent(parentContext);
    for(auto i : children){
      Component_Base::Render(i, screen);
    };
  }

  // using Component_Base::Component_Base;

  App::App()  {
    $state = $initialState();
    assignChildren({
      new TabView()
    });
  }

  unordered_map<string,string> App::$initialState() {
    return {{"labelText", "HELLO THERE"}};
  };
  void screen()
  {
    auto app = new App();
    app->render(nullptr);
  }
