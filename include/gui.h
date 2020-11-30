#ifndef GUI_H
#define GUI_H

#include "api.h"
#include <vector>
#include <array>
#include <ostream>
#include <sstream>
#include <iostream>
#include <map>
extern int auton_sel;

using Run_F = std::function<void(void)>;

namespace GUI
{
    enum Screens
    {
        Home,
        Logs,
        LogDetails,
        Console,
        Sliders,
    };
    

    enum class Module
    {
        CONSOLE = 0,
        CONTROLLER,
        DRIVE,
        DEBUG,
        SYSTEM
    };

    static std::map<Module, std::string> ModuleName = 
    {
        {Module::CONSOLE, "CONSOLE"},
        {Module::CONTROLLER, "CONTROLLER"},
        {Module::DRIVE, "DRIVE"},
        {Module::DEBUG, "DEBUG"},
        {Module::SYSTEM, "SYSTEM"}
    };
    
    class Log_Message
    {
        public: 
        std::string title;
        std::string contents;
        Module source;
        int created;
        Log_Message(std::string title, Module source = Module::DEBUG, std::string contents = "");
    };


    class Console_Message
    {
        public:
        std::string contents;
        int line;
        Module source;
        Console_Message(std::string contents = "", Module source = Module::DEBUG);
    };

    class Slider_Message
    {
        public: 
        std::string title;
        float value;
        Module source;
        Slider_Message(std::string title = "", float value = 0, Module source = Module::DEBUG);
    };
    extern void Console_Log(Console_Message message, int line);
    extern void Slider(Slider_Message message);
    extern Screens ActiveScreen;
    static std::vector<Log_Message> Message_Log;
    static std::vector<Slider_Message> Slider_Log;
    static std::array<GUI::Console_Message, 8> CurrentConsole = {};
    
    static bool PauseLog = false;
    
    extern GUI::Log_Message ActiveLog;

    extern void Set_Screen(Screens screen);
    extern void Render_Home(lv_obj_t *screen);
    extern void Render_Logs(lv_obj_t *screen);
    extern void Render_Sliders(lv_obj_t *screen);
    extern void Render_Log_Details(lv_obj_t *screen);
    extern void Render_Console(lv_obj_t *screen);

    extern void Log(Log_Message message);

    extern lv_obj_t *Assign_Button(lv_obj_t *par, const lv_obj_t *copy, Run_F* run_fn);
    extern lv_res_t Button_Click(lv_obj_t *btn);

    extern void Initialize_Log(void);

    extern lv_obj_t *Render_Default(void);
}; // namespace GUI

#endif // GUI_H
