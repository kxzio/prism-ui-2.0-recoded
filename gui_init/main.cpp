#define _CRT_SECURE_NO_WARNINGS

#include "included.h"
#include "create.h"
#include "window_process.h"
#include <vector>
#include <sstream>
#include "../fnv1a.h"
#include <time.h>
#include "../gui/gui.h"

using namespace ImGui;
float screen_max[]{ GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };

size window_size = size(720, 620);

int main()
{
    gui::get().add_font("segoeui", 21);

    gui::get().set_window_type(gui::get().types_of_window::TITLED);
    /* First argument if window pos - position, where will be rendered window and size, how big window will be */
    window.init( pos{ 0, 0 }, size{ window_size.x, window_size.y } );
   

    /* DIRECTX & wndproc initialization */
    m_directx.  init32(); 
    window.     process_msg();
    /* code shutdown  UPD: fixed crash in time of process closing & (device lost) */ 
    m_directx.  finish();

    return 0;
};

void menu::init() {
   
    //debug background
    ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(0, 0), ImVec2(4000, 4000), ImColor(25, 25, 25), 0.0);

    //start
    gui::get().go();

    //style :::

    //gui solve : 
    gui::get().begin_frame("Framework", pos(0, 0), window_size, __flags::no_move);
    {

        gui::get().offset_brush_pos(pos(15, 15));
        gui::get().begin_child_window("Settings", size(262, 427));
        {
            //child elements
            gui::get().text("Elements examples : ");

            static bool bln = true;
            gui::get().checkbox("Checkbox active", &bln);

            static float test_fl;
            gui::get().slider_float("Float slider", &test_fl, 100);

            static int test_int;
            gui::get().slider_int("Integer slider", &test_int, 100);

            static int combo_int;
            const char* combo_text_[]{ "Selection1", "Selection2", "Selection3" };
            gui::get().combo("Combo selection", &combo_int, combo_text_, ARRAYSIZE(combo_text_));

            gui::get().button("Button");

            gui::get().button("Button 2");

            gui::get().button("Button 3");

            gui::get().button("Button 4");


            static float col[4] {1.f, 0.f, 0.f, 1.f};
            gui::get().color_picker("Colorpicker", col);

            static bool bln3 = false;
            gui::get().checkbox("Another checkbox", &bln3);
            {
                static std::string input;
                gui::get().input("Text input", input);

            }
            gui::get().color_picker("Colorpicker", col);

            static bool bln5 = false;
            gui::get().checkbox("Another checkbox", &bln3);
            {
                static std::string input;
                gui::get().input("Text input", input);

            }
            gui::get().color_picker("Colorpicker", col);

            static bool bln4 = false;
            gui::get().checkbox("Another checkbox", &bln3);
            {
                static std::string input;
                gui::get().input("Text input", input);

            }


        }
        gui::get().end();


        gui::get().offset_brush_pos(pos(15 + 262 + 20, 15));
        gui::get().begin_child_window("Conditions", size(262, 427));
        {
            static int list_element;
            const char* list_text[]{ "Selection1", "Selection2", "Selection3"};
            gui::get().list("Listbox", &list_element, list_text, ARRAYSIZE(list_text));

            const char* textbox[] 
            { 
                "Hello, it is textbox" 
            };
            gui::get().text_box("Textbox", textbox, ARRAYSIZE(textbox));
            static bool v;
            gui::get().toggle_button("Toggle button", &v);
            static int key, mode;
            gui::get().hotkey("Hotkey", &key, &mode);

            static bool multibox_test[5];
            const char* combo_text_[]{ "Selection1", "Selection2", "Selection3" };
            gui::get().multi("Multibox selection", multibox_test, combo_text_, ARRAYSIZE(combo_text_));

            gui::get().button("Button");

            gui::get().button("Button 2");

            gui::get().button("Button 3");

            gui::get().button("Button 4");


            static bool list_element2[10];
            const char* list_text2[]{ "Selection1", "Selection2", "Selection3" , "Selection4", "Selection5", "Selection6", "Selection7" };
            gui::get().multilist("Multiple listbox", list_element2, list_text2, ARRAYSIZE(list_text2));
        }
        gui::get().end();


    }
    gui::get().end();




}

