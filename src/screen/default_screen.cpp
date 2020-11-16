#include "api.h"
#include "gui.h"
#include "main.h"

lv_obj_t* GUI::Render_Default(void)
{
    auto g_btn_region = lv_obj_create(lv_scr_act(), NULL);
    lv_obj_set_size(g_btn_region, lv_obj_get_width(lv_scr_act()),
                    lv_obj_get_height(lv_scr_act()) * 0.9);
    lv_obj_align(g_btn_region, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_obj_set_style(g_btn_region, &lv_style_pretty_color);

    //
    auto g_sb_region = lv_obj_create(lv_scr_act(), NULL);
    lv_obj_set_size(g_sb_region, lv_obj_get_width(lv_scr_act()),
                    lv_obj_get_height(lv_scr_act()) * 0.1);
    lv_obj_align(g_sb_region, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
    lv_obj_set_style(g_sb_region, &lv_style_pretty_color);

    auto HomeFn = new Run_F([](void) -> void {
        Set_Screen(Home);
    });
    auto home_button = GUI::Assign_Button(g_sb_region, NULL, HomeFn);
    lv_obj_set_style(home_button, &lv_style_pretty_color);
    lv_obj_align(home_button, NULL, LV_ALIGN_CENTER, 0, 0);

    auto g_sb_label = lv_label_create(home_button, NULL);
    lv_obj_set_style(g_sb_label, &lv_style_pretty_color);
    lv_obj_align(g_sb_label, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(g_sb_label, "HOME");
    return g_btn_region;
}