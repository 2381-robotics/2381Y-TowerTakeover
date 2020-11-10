#include "screen/app.hpp"
#include "api.h"
#include "button.hpp"
#include "label.hpp"
#include "tabview.hpp"
#include "utils.hpp"
#include "tab.hpp"

#include <unordered_map>
#include <functional>
#include <vector>

using namespace std;

Tab::Tab(string name, vector<Component_Base*>) : name(name), Component_Base(children)
{

}

lv_obj_t* Tab::createComponent(lv_obj_t* context)
{
 auto tabview = lv_tabview_add_tab(context, "hello");
 return tabview;
}