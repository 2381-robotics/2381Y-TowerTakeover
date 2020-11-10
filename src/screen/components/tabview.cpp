#include "screen/app.hpp"
#include "api.h"
#include "button.hpp"
#include "label.hpp"
#include "tabview.hpp"
#include "utils.hpp"

#include <unordered_map>
#include <functional>
#include <vector>


using namespace std;
static lv_style_t style_box;



lv_obj_t* TabView::createComponent(lv_obj_t* context)
{
 auto tabview = lv_tabview_create(lv_scr_act(), NULL);
 auto tab1 = lv_tabview_add_tab(tabview, "REE");
 auto label =  lv_label_create(tabview, NULL);
lv_label_set_text(label, "hei");
//  return lv_scr_act();
 return tabview;
}


