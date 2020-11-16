#include "gui.h"
#include "main.h"
#include "api.h"
#include "utils.hpp"

using namespace std;
using namespace GUI;

Log_Message GUI::ActiveLog = Log_Message("");

void GUI::Render_Log_Details(lv_obj_t* screen)
{
    auto log = GUI::ActiveLog;
    
    print(log.title + "REEEE");

    auto title_label = lv_label_create(screen, NULL);
    lv_label_set_text(title_label, ("LOGS: " + log.title).c_str());
    lv_obj_align(title_label, NULL, LV_ALIGN_IN_TOP_MID, 0, 5);

    auto source_label = lv_label_create(screen, NULL);
    lv_label_set_text(source_label, (GUI::ModuleName[log.source]).c_str());
    lv_obj_align(source_label, NULL, LV_ALIGN_IN_TOP_LEFT, 20, 20);

    auto time_label = lv_label_create(screen, NULL);
    lv_label_set_text(time_label, (to_string(log.created) + "ms").c_str());
    lv_obj_align(time_label, NULL, LV_ALIGN_IN_TOP_LEFT, 20, 40);

    auto contents_label = lv_label_create(screen, NULL);
    lv_label_set_text(contents_label, (log.contents).c_str());
    lv_obj_align(contents_label, NULL, LV_ALIGN_IN_TOP_LEFT, 20, 60);
}