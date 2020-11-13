#ifndef GUI_H
#define GUI_H

#include "api.h"
extern int auton_sel;

namespace GUI
{
    enum Screens
    {
        Home,
        Logs,
    };

    extern void Set_Screen(Screens screen);
    extern void Render_Home(lv_obj_t *screen);
    extern void Render_Logs(lv_obj_t *screen);

    extern lv_obj_t *Render_Default(void);
}; // namespace GUI

// void gui(void);
void rerender(void);
void pls(void);
#endif // GUI_H
