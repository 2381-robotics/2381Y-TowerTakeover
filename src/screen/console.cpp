#include "gui.h"
#include "main.h"
#include "api.h"
#include "utils.hpp"

using namespace std;
using namespace GUI;

GUI::Console_Message::Console_Message( string contents, GUI::Module source) : source(source), contents(contents)
{
}
void GUI::Console_Log(Console_Message message, int line)
{
    auto previousMessage = CurrentConsole[line];
    CurrentConsole[line] = message;
    if (ActiveScreen == GUI::Screens::Console && CurrentConsole[line].contents != previousMessage.contents)
    {
        Set_Screen(Screens::Console);

    }
    print(CurrentConsole[line].contents);
    print(previousMessage.contents);
    
}
void GUI::Render_Console(lv_obj_t *screen)
{
    auto line1 = lv_label_create(screen, NULL);
    lv_label_set_text(line1, (CurrentConsole[0].contents).c_str());
    lv_obj_align(line1, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 5);

    auto line2 = lv_label_create(screen, NULL);
    lv_label_set_text(line2, (CurrentConsole[1].contents).c_str());
    lv_obj_align(line2, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 25);
    
     auto line3 = lv_label_create(screen, NULL);
    lv_label_set_text(line3, (CurrentConsole[2].contents).c_str());
    lv_obj_align(line3, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 45);

    auto line4 = lv_label_create(screen, NULL);
    lv_label_set_text(line4, (CurrentConsole[3].contents).c_str());
    lv_obj_align(line4, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 65);

     auto line5 = lv_label_create(screen, NULL);
    lv_label_set_text(line5, (CurrentConsole[4].contents).c_str());
    lv_obj_align(line5, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 85);

    auto line6 = lv_label_create(screen, NULL);
    lv_label_set_text(line6, (CurrentConsole[5].contents).c_str());
    lv_obj_align(line6, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 105);

     auto line7 = lv_label_create(screen, NULL);
    lv_label_set_text(line7, (CurrentConsole[6].contents).c_str());
    lv_obj_align(line7, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 125);

    auto line8 = lv_label_create(screen, NULL);
    lv_label_set_text(line8, (CurrentConsole[7].contents).c_str());
    lv_obj_align(line8, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 145);


}