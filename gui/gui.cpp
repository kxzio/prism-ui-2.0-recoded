#include "gui.h"
#include "../menu.h"
#include <map>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "version.h"
#include <iomanip>
#include <conio.h>
#include <stdio.h>

//MAIN FILE OF XGUI (PrismaticGui)

/*


			██████╗░██████╗░██╗░██████╗███╗░░░███╗  ░██████╗░██╗░░░██╗██╗
			██╔══██╗██╔══██╗██║██╔════╝████╗░████║  ██╔════╝░██║░░░██║██║
			██████╔╝██████╔╝██║╚█████╗░██╔████╔██║  ██║░░██╗░██║░░░██║██║
			██╔═══╝░██╔══██╗██║░╚═══██╗██║╚██╔╝██║  ██║░░╚██╗██║░░░██║██║
			██║░░░░░██║░░██║██║██████╔╝██║░╚═╝░██║  ╚██████╔╝╚██████╔╝██║
			╚═╝░░░░░╚═╝░░╚═╝╚═╝╚═════╝░╚═╝░░░░░╚═╝  ░╚═════╝░░╚═════╝░╚═╝

			-  prismatic gui framework code v2.15
			-  first date of creation			 :    = Mar 8, 2021 =

			- old repos							 : https://github.com/kxzio/prism_ui) v1.2
			- current repos					     : TODO ( IN PROCESS )
			- created  by						 : spiritualdoxy ( Aleksander Vasilevskii )

			- last edited						 : 24.10.2024


			//// TODO LIST ////							Progress

			1.	Recode WINAPI window creation			   ✓
			2.	Recode font creation system				   ✓
			3.	Delete TrueType and add TrueType		   ✓
			4.	Recode Style creating and make it easier   ✓   ( Not Almost / Styles not colorized and no circled styles )
			5.	Gui scale								   ✓
			6.	Window scrolling function		           ✓   ( Hard asf, i hate this trashcode but it really works so okey )
			7.	Recode sliders and mostly all elements	   ✓
			8.	Minor fixes from 1.2					   ✓
			9.	Toogle Button + Animation				   ✓
			10.	Slider + Animation					       ✓
			11.	Button + Animation					       ✓
			12.	Fix of Combo and MultiBox				   ✓
			13.	Create a multilist					       ✓
			13.	Fix code and make it readable		       ✓
			13.	Make animations to other elements		   🆇
			13.	Make TextBox use more keyboard keys	       🆇
			13.	Make User-Friendly Recode			       🆇
			13.	Delete a background to Combos and etc      🆇


*/

using namespace ImGui;


//keys
const char* const key_names[]       = {

	//other :::
	"Unknown","LM","RM","Cancel","M3","X1 mouse","X2 mouse","Unknown","Back","Tab","Unknown","Unknown","Clear","Return","Unknown","Unknown","Shift","Ctrl","Menu", //19
	"Pause","Capital","KANA","Unknown","VK_JUNJA","VK_FINAL","VK_KANJI","Unknown","Escape","Convert","NonConvert","Accept","VK_MODECHANGE","Space","Prior",
	"Next","End","Home","Left","Up","Right","Down","Select","Print","Execute","Snapshot","Insert","Delete","Help", //48
	//input keys :::
	"0","1","2","3","4","5","6","7","8","9"," "," "," "," "," "," "," ","a","b","c","d","e",
	"f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z",
	//other:::
	"Win left","Win right","Apps","Unknown","Sleep","Numpad 0","Numpad 1","Numpad 2","Numpad 3","Numpad 4",
	"Numpad 5","Numpad 6","Numpad 7","Numpad 8","Numpad 9","Multiply","Add","Seperator","Subtract","Decimal",
	"Devide","F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","F11","F12","F13","F14","F15","F16","F17","F18",
	"F19","F20","F21","F22","F23","F24","Unknown","Unknown","Unknown","Unknown","Unknown","Unknown","Unknown","Unknown",
	"Numlock","Scroll","VK_OEM_NEC_EQUAL","VK_OEM_FJ_MASSHOU","VK_OEM_FJ_TOUROKU","VK_OEM_FJ_LOYA","VK_OEM_FJ_ROYA","Unknown",
	"Unknown","Unknown","Unknown","Unknown","Unknown","Unknown","Unknown","Unknown","Shift left","Shift right","Ctrl left",
	"Ctrl right","Left menu","Right menu"
};
const char* const other_key_names[] = {
	//48 + 
	")","!","@","#","$","%","^","&","*","(", "~", "_", "+", "{", "}", ":", "|", "<", ">", "?"
};
/*
////////////////////////////////////////////////////////////////////
*/


//handle operation for imgui datatypes
ImVec2 operator-(const ImVec2& l, const ImVec2& r) { return{ l.x - r.x, l.y - r.y }; }
ImVec2 operator+(const ImVec2& l, const ImVec2& r) { return{ l.x + r.x, l.y + r.y }; }
/*
////////////////////////////////////////////////////////////////////
*/


//gui scale. This param makes window elements bigger. But font still have the constant size that have been configurated in main file
static float gui_scale = 1.2;
char change_case(char c) {

	//chaning case of symbols from BIG to low 

	if (std::isupper(c))
		return std::tolower(c);
	else
		return std::toupper(c);
}
/*
////////////////////////////////////////////////////////////////////
*/


//Winapi window configuration
void  gui::set_window_type(int type)
{
	// types_of_window enum
	this->WINDOWS_DATA_type_of_window = type;
}
void  gui::make_it_fullscreen()
{
	//Get screen DC
	HDC hDCScreen = GetDC(NULL);

	//Size of window. It doesnt updating after resolution changing. I think its useless
	int screen_x = GetDeviceCaps(hDCScreen, HORZRES);
	int screen_y = GetDeviceCaps(hDCScreen, VERTRES);

	this->WINDOWS_DATA_window_pos = ImVec2(0, 0);
	this->WINDOWS_DATA_window_size = ImVec2(screen_x, screen_y);
	this->WINDOWS_DATA_type_of_window = types_of_window::POPUP;


	ReleaseDC(NULL, hDCScreen);
	//Dont forget to release dc cause of memory leak

}
/*
////////////////////////////////////////////////////////////////////
*/


//color modulations
ImColor hex_to_color(const std::string& hexColor) {
	//Transform HEX color code to ImColor class
	int r = 0, g = 0, b = 0;
	if (hexColor[0] == '#') {
		sscanf_s(hexColor.c_str(), "#%02x%02x%02x", &r, &g, &b);
	}
	return ImColor(r, g, b);
}
/*
////////////////////////////////////////////////////////////////////
*/


//HSV modulations
ImColor change_s_of_color(ImColor text_color, float s)
{
	//HSV - Hue, Saturation, Value. In this function we change Saturation of RGB value

	//Converting to RGB
	float text_color_h, text_color_s, text_color_v;
	ImGui::ColorConvertRGBtoHSV(text_color.Value.x, text_color.Value.y, text_color.Value.z, text_color_h, text_color_s, text_color_v);

	//Changing RGB TO HSV backwards
	float text_color_r, text_color_g, text_color_b;
	ImGui::ColorConvertHSVtoRGB(text_color_h, s / 100.f, text_color_v, text_color_r, text_color_g, text_color_b);

	//back RGB color, with already modulated Saturation
	return ImColor(text_color_r, text_color_g, text_color_b);
}
ImColor change_v_of_color(ImColor text_color, float v)
{
	//HSV - Hue, Saturation, Value. In this function we change Value of HSV value
	float text_color_h, text_color_s, text_color_v;
	//Transform Color to HSV
	ImGui::ColorConvertRGBtoHSV(text_color.Value.x, text_color.Value.y, text_color.Value.z, text_color_h, text_color_s, text_color_v);

	//Transform HSV to color backwards
	float text_color_r, text_color_g, text_color_b;
	ImGui::ColorConvertHSVtoRGB(text_color_h, text_color_s, v / 100.f, text_color_r, text_color_g, text_color_b);
	//Returning of RGB color already modulated 
	return ImColor(text_color_r, text_color_g, text_color_b);
}
ImColor inverse_color(ImColor clr)
{
	//Just inverting the colors from the base value ( 255 )
	return ImColor(int(255 - clr.Value.x * 255), int(255 - clr.Value.y * 255), int(255 - clr.Value.z * 255), int(clr.Value.w * 255));
}
/*
////////////////////////////////////////////////////////////////////
*/


//Style application and configuration
void	      apply_style(int themes)
{
	//Creating a ready-made styles 
	ImColor main_color, additional_color, background_color, text_color;

	switch (themes)
	{
		case 0:
		{
			main_color = hex_to_color("#3B1E54");
			additional_color = hex_to_color("#9B7EBD");
			background_color = hex_to_color("#D4BEE4");
			text_color = hex_to_color("#EEEEEE");
		}
		break;
		case 1:
		{
			main_color = hex_to_color("#9A7E6F");
			additional_color = hex_to_color("#CBD2A4");
			background_color = hex_to_color("#E9EED9");
			text_color = hex_to_color("#54473F");
		}
		break;
		case 2:
		{
			main_color = hex_to_color("#e91e63");
			additional_color = hex_to_color("#ed4b82");
			background_color = hex_to_color("#000000");
			text_color = hex_to_color("#FFFFFF");
		}
		break;
		case 3:
		{
			main_color = hex_to_color("#533473");
			additional_color = hex_to_color("#533473");
			background_color = hex_to_color("#ffffff");
			text_color = hex_to_color("#533473");
		}
		break;
		case 5:
		{
			main_color = hex_to_color("#e91e63");
			additional_color = hex_to_color("#ed4b82");
			background_color = hex_to_color("#FFFFFF");
			text_color = hex_to_color("#000000");
		}
		break;
	}

	//Modulating style color using pre-made colors

	gui::get().__colorstyle(__style_color::text)				= text_color;
	gui::get().__colorstyle(__style_color::permissible_text)	= ImColor(text_color.Value.x, text_color.Value.y, text_color.Value.z, 0.6f);
	gui::get().__colorstyle(__style_color::window_background)	= background_color;
	gui::get().__colorstyle(__style_color::childwindow_bg)		= ImColor(additional_color.Value.x, additional_color.Value.y, additional_color.Value.z, 0.0f);;
	gui::get().__colorstyle(__style_color::titlebar_background) = ImColor(main_color.Value.x, main_color.Value.y, main_color.Value.z, 0.3f);
	gui::get().__colorstyle(__style_color::window_oultine)		= ImColor(main_color.Value.x, main_color.Value.y, main_color.Value.z, 1.f);
	gui::get().__colorstyle(__style_color::element_outline)		= ImColor(main_color.Value.x, main_color.Value.y, main_color.Value.z, 0.3f);

	//Elements
	gui::get().__colorstyle(__style_color::frame_default)		= ImColor(main_color.Value.x, main_color.Value.y, main_color.Value.z, 0.3f);
	gui::get().__colorstyle(__style_color::frame_hovered)		= ImColor(main_color.Value.x, main_color.Value.y, main_color.Value.z, 0.4f);
	gui::get().__colorstyle(__style_color::frame_active)		= ImColor(main_color.Value.x, main_color.Value.y, main_color.Value.z, 0.5f);
	gui::get().__colorstyle(__style_color::selectable_default)	= background_color;
	gui::get().__colorstyle(__style_color::selectable_hovered)	= ImColor(main_color.Value.x, main_color.Value.y, main_color.Value.z, 0.3f);
	gui::get().__colorstyle(__style_color::selectable_active)	= ImColor(main_color.Value.x, main_color.Value.y, main_color.Value.z, 1.f);
	
	//Objects
	gui::get().__colorstyle(__style_color::active_obj)			= ImColor(main_color.Value.x, main_color.Value.y, main_color.Value.z, 0.7f);
	gui::get().__colorstyle(__style_color::unactive_obj)		= ImColor(main_color.Value.x, main_color.Value.y, main_color.Value.z, 0.1f);

	//Window X, Y Padding
	gui::get().__style	   (__style_value::window_padding_x)	= 8.f;
	gui::get().__style     (__style_value::window_padding_y)	= 15.f;

	//Item X, Y Spacing
	gui::get().__style     (__style_value::item_spacing_x)		= 12.f;
	gui::get().__style     (__style_value::item_spacing_y)		= 8.f;

}
ImColor& gui::__colorstyle(__style_color i) {
	return style.at(i);
}
float& gui::  __style(__style_value i) {
	return style_quantity.at(i);
}
ImColor gui:: get_frame_color(bool hovered, bool active) {

	//Desition for frame colors (active, default, hovered)

	if (active)
		return __colorstyle(__style_color::frame_active);

	if (hovered && this->is_able_to_click)
		return __colorstyle(__style_color::frame_hovered);

	return __colorstyle(__style_color::frame_default);
}
/*
////////////////////////////////////////////////////////////////////
*/


//Gui font system - using vectors for fonts
void gui::add_font(const char* font, int size_of_font)
{
	//creating a font class, that consist of font name, font_size and etc.
	font_modulation_class new_font;
	new_font.font_mode = new_font.mode_of_adding_font::NAME;

	new_font.font_name = font;
	new_font.size_of_font = size_of_font;

	ImFont* font_date = new ImFont();
	new_font.font_addr = font_date;

	fonts.push_back(new_font);

	delete font_date;

}
void gui::add_font(void* font_data, int font_data_size, float size_of_font)
{
	//creating a font class, that consist of font name, font_size and etc.
	font_modulation_class new_font;
	new_font.font_mode = new_font.mode_of_adding_font::MEMORY;

	new_font.font_data = font_data;
	new_font.font_data_size = font_data_size;
	new_font.size_of_font = size_of_font;

	ImFont* font_date = new ImFont();
	new_font.font_addr = font_date;

	fonts.push_back(new_font);

	delete font_date;

}
/*
////////////////////////////////////////////////////////////////////
*/


//Scroll functions for windows
float      set_or_get_scroll_data(gui_id id, float value = -1488.f)
{
	//map vector that has information about window element height

	static std::map<gui_id, float> window_pos;
	//last id 
	//get itp for checking
	auto vpos = window_pos.find(id); // current itp
	//contract 
	if (vpos == window_pos.end()) {
		window_pos.insert({ id, 0.f });
		//insert perfect id
		vpos = window_pos.find(id);
		// find id
	}
	if (value != -1488.f)
	{
		vpos->second = value;
	}
	return vpos->second;
}
int        set_or_get_window_change_triggers(gui_id id, int value = -1488)
{
	//Map vector that has information about changing triggers, when we have to rebuild something

	//dt of id
	static std::map<gui_id, int> window_pos;
	//last id 
	//get itp for checking
	auto vpos = window_pos.find(id); // current itp
	//contract 
	if (vpos == window_pos.end()) {
		window_pos.insert({ id, 0 });
		//insert perfect id
		vpos = window_pos.find(id);
		// find id
	}
	if (value != -1488)
	{
		vpos->second = value;
	}
	return vpos->second;
}
float gui::count_abillity_to_scroll(pos size_of_window)
{
	//Not working yet function. I recoded Scroll function. This function have to be rebuilded or deleted
	if (brush_pos.y < size_of_window.y)
	{
		set_or_get_scroll_data(count_of_windows, 0.f);
		return false;
	}
	else
	{
		set_or_get_scroll_data(count_of_windows, brush_pos.y - size_of_window.y);
		return true;
	}

}

/*
////////////////////////////////////////////////////////////////////
*/


//Main window functions
void gui::go() {

	//Count of windows - is easy getting ID that we use for some window-personal infomation
	count_of_windows = 0;

	//window start pos/size data
	this->pos_parameter.clear();
	//window counter
	this->window_manifold.clear();
	//window drag-target
	if (!dont_restore_drag_target)
		this->drag_target_id = NULL;
	//brush pos clear
	window_poses.clear();
	//old data clear
	this->old_window_manifold.clear();
	//combo/multi click abl
	this->is_able_to_click = true;
	this->last_hovered_window = NULL;
}
void gui::think(c_window_form* __this, int flags)	 {

	//Changing count of windows ID, since we created a new window
	count_of_windows += 1;

	//Aplying current style that we already pre-created
	apply_style(5);

	//get drawlist data to render last updted vertexes
	auto dl_data = ImGui::GetBackgroundDrawList(); 
	
	//ID for window_form_internal rebuild
	auto const perfect_window_id = get_propper_id(__this->name_str);

	//Check if is window - child
	const bool is_child = flags & child;

	//read last begin : pos, size
	c_pos_parameters pos_settings = this->pos_parameter.at(this->pos_parameter.size() - 1);

	//Move brush inside of child
	if (is_child)
		pos_settings.pos = __brush_pos();

	//Drag target ID
	static std::map<gui_id, pos> window_pos;
	auto const id = get_propper_id(__this->name_str);
	auto vpos = window_pos.find(id); // current itp
	//contract 
	if (vpos == window_pos.end()) {
		window_pos.insert({ id, pos_settings.pos }); 
		//insert perfect id
		vpos = window_pos.find(id); 
		// find id
	}

	auto tmp_scroll = global_mouse_scroll;

	if (is_child)
		vpos->second = __brush_pos();

	//use much more conveniently data-type
	c_internal_window_form wform_old { __this->name_str, perfect_window_id, pos_settings.pos, pos_settings.size };
	wform_old.flags = flags;

	//add to library new object
	this->old_window_manifold.push_back(&wform_old);

	if (!wform_old.lock_menu) {
		//move RECT by mouse event dx
		if (move_rect_by_mouse(__this->name_str, &vpos->second, size(pos_settings.size.x, 30))) {
			this->drag_target_id = perfect_window_id;
		}
	}

	//internal window form rebuild :::

	//restore and update all data from old class to new 
	pos_settings.pos = vpos->second;

	//final rebuild for cursor, the most important part of this code
	__brush_pos() = pos_settings.pos;

	//
	static std::map<gui_id, float> last_scroll_map;
	//get itp for checking
	auto last_scroll = last_scroll_map.find(id); // current itp
	//contract 
	if (last_scroll == last_scroll_map.end()) {
		last_scroll_map.insert({ id, 0 });
		//insert perfect id
		last_scroll = last_scroll_map.find(id);
		// find id
	}

	//THIS IS DEBUG CODE, THAT SHOWS UP A DEBUG WINDOW OF SCROLL INFO
	//Text(std::to_string(set_or_get_scroll_data(count_of_windows)).c_str());

	float max_scroll = (set_or_get_scroll_data(count_of_windows) - pos_settings.size.y);
	//Scroll processing
	if (is_hovered(pos_settings.pos, pos_settings.pos + pos_settings.size)) {
		if (ImGui::GetIO().MouseWheel < 0.0f) {
			if (max_scroll > 0) {
				last_scroll->second -= ImGui::GetIO().MouseWheel * 45;
				last_scroll->second = std::clamp(last_scroll->second, 0.0f, max_scroll);
			}
		}
		if (ImGui::GetIO().MouseWheel > 0.0f && max_scroll > 0) {
				last_scroll->second -= ImGui::GetIO().MouseWheel * 45;
				last_scroll->second = std::clamp(last_scroll->second, 0.0f, max_scroll);
		}
	}

	//Scroll data animation
	float target_scroll = last_scroll->second;
	float animated_scroll = lerp_animate(__this->name_str, "scroll_animation", true, last_scroll->second, 13, INTERP);
	
	//use much more conveniently data-type
	c_internal_window_form wform_full { __this->name_str, perfect_window_id, pos_settings.pos, pos_settings.size };

	wform_full.flags = flags;
	scroll_data[count_of_windows] = animated_scroll;
	//pushing the scroll data of window

	//add to library new object
	this->window_manifold.push_back(&wform_full);

	//Focus on current window
	focus_handle(perfect_window_id);

	//Background
	dl_data->AddRectFilled(pos_settings.pos, pos_settings.pos + pos_settings.size, !is_child ? __colorstyle(__style_color::window_background) : __colorstyle(__style_color::childwindow_bg), 0.0);

	//Drawing a shadows for scrolls
	int alpha_for_down_gradient  = lerp_animate(__this->name_str, "down_grad",  set_or_get_scroll_data(count_of_windows) > 15.f, 40, 20, DYNAMIC);
	int alpha_for_upper_gradient = lerp_animate(__this->name_str, "upper_grad", animated_scroll > 15.f,						   40,   20,   DYNAMIC);

	//Drawing shadows
	GetForegroundDrawList()->AddRectFilledMultiColor(pos(pos_settings.pos.x, pos_settings.pos.y + pos_settings.size.y - 11 * gui_scale), pos(pos_settings.pos.x + pos_settings.size.x, pos_settings.pos.y + pos_settings.size.y), ImColor(0, 0, 0, 0), ImColor(0, 0, 0, 0), ImColor(0, 0, 0, alpha_for_down_gradient), ImColor(0, 0, 0, alpha_for_down_gradient));
	GetForegroundDrawList()->AddRectFilledMultiColor(pos(pos_settings.pos.x, pos_settings.pos.y), pos(pos_settings.pos.x + pos_settings.size.x, pos_settings.pos.y + 11 * gui_scale), ImColor(0, 0, 0, alpha_for_upper_gradient), ImColor(0, 0, 0, alpha_for_upper_gradient), ImColor(0, 0, 0, 0), ImColor(0, 0, 0, 0));

	//Processing last_hovered
	auto dt = get_current_manifold_lib();
	// nya
	if (is_hovered(dt->pos, dt->pos + get_pos_settings()->size)) {
		last_hovered_window = id;
	}

	//Draw title bar and etc.
	draw_window_features();

	// fix 05.03 old_window form rebuild
	wform_old = wform_full;
	
	//Get keyboard input
	auto i_input = _getch();
	
	// Pushing clip RECT to make scroll function hide useless elements that we cant see
	dl_data->PushClipRect(pos_settings.pos, pos_settings.pos + pos_settings.size, true);

	//Brush pos configuration
	plus_brush_pos(pos(__style(window_padding_x), __style(window_padding_y)));

	//Final use of scroll data, Now we scroll all the information in window
	plus_brush_pos(pos(0, -animated_scroll));

	window_poses.push_back(pos(wform_full.pos));

}
void gui::draw_window_features() {

	//combat
	//get drawlist data to render last updted vertexes
	auto dl_data = ImGui::GetBackgroundDrawList();
	auto window_data = get_current_manifold_lib();
	auto pos_settings = get_pos_settings();

	if (!(window_data->flags & __flags::no_titlebar)) {

		//title bar bg
		dl_data->AddRectFilled(window_data->pos, window_data->pos + pos(pos_settings->size.x, 30), __colorstyle(__style_color::titlebar_background));
		//titlebar text
		dl_data->AddText(window_data->pos + pos(8, 8), __colorstyle(__style_color::text), window_data->name_str.c_str());
	}

	//outline
	dl_data->AddRect(window_data->pos, window_data->pos + pos_settings->size, __colorstyle(__style_color::window_oultine), 0.0);

	//overlay child title
	if (window_data->flags & __flags::child) {
		//title 
		pos pos22 = window_data->pos + pos((pos_settings->size.x / 2) - CalcTextSize(window_data->name_str.c_str()).x / 2 - 3 * gui_scale, -CalcTextSize(window_data->name_str.c_str()).y / 2);
		dl_data->AddRectFilled(pos22, pos22 + pos(CalcTextSize(window_data->name_str.c_str()).x + 6 * gui_scale, CalcTextSize(window_data->name_str.c_str()).y), __colorstyle(__style_color::window_background));
		dl_data->AddText(window_data->pos + pos((pos_settings->size.x / 2) - CalcTextSize(window_data->name_str.c_str()).x / 2, -CalcTextSize(window_data->name_str.c_str()).y / 2), __colorstyle(__style_color::text), window_data->name_str.c_str());
	}


}
void gui::begin_child_window(const char* str, size sz, int flags) {
	//draw 
	begin_frame(str, pos(), size(sz.x * gui_scale, sz.y * gui_scale), __flags::no_move | child | flags);

}
void gui::begin_frame(std::string __strname, pos start_pos, size start_size, int flags) {

	//gui solve : 
	c_window_form __this_frame(__strname.c_str());
	//update 
	gui::get().set_window_config(start_pos, start_size);
	//handle class
	gui::get().think(&__this_frame, flags);

}
void gui::end() {

	//if (set_or_get_scroll_data(count_of_windows) == 0.0f)
	set_or_get_scroll_data(count_of_windows, __brush_pos().y + scroll_data[count_of_windows]);

	//gui::get().__brush_pos() = pos(get_pos_settings()->pos.x + 0, get_pos_settings()->pos.y + get_pos_settings()->size.y);
	//plus_brush_pos(pos(__style(window_padding_x), __style(window_padding_y)));

	this->window_manifold.pop_back();
	this->old_window_manifold.pop_back();
	this->window_poses.pop_back();
	GetBackgroundDrawList()->PopClipRect();


}
/*
////////////////////////////////////////////////////////////////////
*/


//mainfolds for current window (get id)
c_internal_window_form* gui::get_current_manifold_lib() {
	return this->window_manifold.at(window_manifold.size() - 1);
}
c_internal_window_form* gui::get_old_manifold_lib() {
	return this->old_window_manifold.at(old_window_manifold.size() - 1);
}
c_pos_parameters*	    gui::get_pos_settings() {
	return &this->pos_parameter.at(pos_parameter.size() - 1);
}
void gui::					 set_window_config(pos p1, pos p2) {
	this->pos_parameter.push_back({ p1, p2 });
}

/*
////////////////////////////////////////////////////////////////////
*/


//brush settings (pos, offsets)
void gui::offset_brush_pos(pos p) {
	__brush_pos() = window_poses.at(window_poses.size() - 1) + ImVec2(p.x * gui_scale, p.y * gui_scale);
}
void gui::plus_brush_pos(pos p) {
	__brush_pos() = __brush_pos() + ImVec2(p.x * gui_scale, p.y * gui_scale);
}
pos& gui::__brush_pos() {

	return this->brush_pos;
}
/*
////////////////////////////////////////////////////////////////////
*/


//Utils for elements and windows
bool gui::  is_hovered(pos p1, pos p2, bool popup_status) {

	//combat
	return IsMouseHoveringRect(p1, p2, false);
}
void gui::  focus_handle(gui_id id) {

	//get current lib data
	auto dt = get_current_manifold_lib();
	// nya
	if (is_clicked_once() && is_hovered(dt->pos, dt->pos + get_pos_settings()->size)) {
		//drag if window is not moving and we havent got any drag target
		if (drag_target_id == NULL && get_old_manifold_lib()->frame_move_speed < 3.5f)
			this->focus_target_id = dt->id;
	}

}
gui_id gui::get_propper_id(std::string buf) {

	//get current seed of frame
	gui_id seed = GImGui->CurrentWindow->IDStack.back();
	//hash id, using generation seed
	gui_id id = ImHashStr(buf.c_str(), NULL, seed);
	//add id to database
	KeepAliveID(id);
	//return generated id
	return id;
}
bool gui::	is_clicked_once(int i) {
	//combat
	return GetIO().MouseClicked[i];
}
bool gui::	is_holding(int i) {
	//combat
	return GetIO().MouseDown[i];
}
bool gui::  move_rect_by_mouse(std::string buf_name, pos* p1, size p2) {

	//check, can we try to move
	if (get_old_manifold_lib()->flags & __flags::no_move)
		return false;

	//boolean return 
	bool __return_adress = false;

	//get last cursor pos
	GetCursorPos(&cursor);

	//dt of id
	static std::map<gui_id, bool> rect_move_data;
	//last id 

	auto const id = get_propper_id(buf_name);
	//get itp for checking
	auto should_drag = rect_move_data.find(id); // current itp
	if (should_drag == rect_move_data.end()) {
		rect_move_data.insert({ id, false }); //insert perfect id
		should_drag = rect_move_data.find(id); // find id
	}

	//dt of id
	static std::map<gui_id, bool> rect_move_data2;

	//get itp for checking
	auto should_move = rect_move_data2.find(id); // current itp
	if (should_move == rect_move_data2.end()) {
		rect_move_data2.insert({ id, false }); //insert perfect id
		should_move = rect_move_data2.find(id); // find id
	}

	//dt of id cursor
	static std::map<gui_id, POINT> cursor_corrected_data;
	//get itp for checking
	auto cursor_corrected = cursor_corrected_data.find(id); // current itp
	if (cursor_corrected == cursor_corrected_data.end()) {
		cursor_corrected_data.insert({ id, cursor }); //insert perfect id
		cursor_corrected = cursor_corrected_data.find(id); // find id
	}

	//in function named "go" we have to clear all vectors and variables, but if we have 1 minimum target
	//we have to ignore restoring, and our another new old/old window will get actual value
	dont_restore_drag_target = false;

	// poses before next tick transform
	int old_x = p1->x, old_y = p1->y;

	if (DEBUG)  //debug background - //history
		ImGui::GetBackgroundDrawList()->AddRectFilled(*p1, *p1 + p2, ImColor(255, 255, 255, 30), 0.0);

	//drag if...
	if (is_holding() && is_hovered(*p1, *p1 + p2)) {

		//nya >-<
		should_drag->second = true;

		//yeah, we soo good
		if (!should_move->second) {
			cursor_corrected->second.x = cursor.x - p1->x;
			cursor_corrected->second.y = cursor.y - p1->y;
			should_move->second = true;
		}

		//set me free
		dont_restore_drag_target = true;
	}

	if (should_move->second && (drag_target_id == NULL || drag_target_id == get_propper_id(buf_name))) {

		if (id == focus_target_id)
		{
			//ok
			*p1 = pos(cursor.x - cursor_corrected->second.x, cursor.y - cursor_corrected->second.y);
			__return_adress = true;
		}
	}


	//unresolved crash
	if (true) {
		//update window move velocity 
		const float poses_diff = sqrt(pow((old_x - p1->x), 2) + pow((old_y - p1->y), 2));
		//update manifold lib, using math:diff between old and new one poses
		this->old_window_manifold.at(old_window_manifold.size() - 1)->frame_move_speed = poses_diff;
	}

	if (!is_holding()) {
		should_drag->second = false;
		should_move->second = false;
	}

	return __return_adress;


}
float gui:: lerp_animate(const char* label, const char* second_label, bool if_, float Maximal_, float Speed_, int type) {

	auto ID = ImGui::GetID((std::stringstream{} << label << second_label).str().c_str());

	static std::map<ImGuiID, float> pValue;

	auto ItPLibrary = pValue.find(ID);

	if (ItPLibrary == pValue.end())
	{
		pValue.insert({ ID, 0.f });
		ItPLibrary = pValue.find(ID);
	}

	const float FrameRateBasedSpeed = Speed_ * (1.f - ImGui::GetIO().DeltaTime);

	switch (type) {

	case DYNAMIC: {
		if (if_) //do
			ItPLibrary->second += abs(Maximal_ - ItPLibrary->second) / FrameRateBasedSpeed;
		else
			ItPLibrary->second -= (0 + ItPLibrary->second) / FrameRateBasedSpeed;
	}
				break;

	case INTERP: {
		ItPLibrary->second += (Maximal_ - ItPLibrary->second) / FrameRateBasedSpeed;
	}
			   break;

	case STATIC: {
		if (if_) //do
			ItPLibrary->second += FrameRateBasedSpeed;
		else
			ItPLibrary->second -= FrameRateBasedSpeed;
	}
			   break;
	}

	if (type != INTERP) {
		//clamp
		if (ItPLibrary->second > Maximal_)
			ItPLibrary->second = Maximal_;
		else if (ItPLibrary->second < 0)
			ItPLibrary->second = 0;
	}

	return ItPLibrary->second;

}
float gui:: calc_frame_w() {
	auto pos_settings = get_pos_settings();
	return get_pos_settings()->size.x - __style(__style_value::window_padding_x) * 2;
}
void gui::  add_element(size sz)
{
	plus_brush_pos(pos(0, sz.y));
	plus_brush_pos(pos(0, __style(item_spacing_y)));
}


/*
////////////////////////////////////////////////////////////////////
*/


//Debug functions
void gui::add_window_debug_log(std::string __str) {

	if (__str.size() == 0)
		return;

	auto data = get_current_manifold_lib();
	GetBackgroundDrawList()->AddText(data->pos + pos(4, 3 + 13 * data->debug_logs_count), ImColor(255, 255, 255, 20), __str.c_str());
	//move it down
	data->debug_logs_count++;
}
/*
////////////////////////////////////////////////////////////////////
*/


//Main gui elements
void gui::	text(const char* __char)			 
{

	auto dl_data = ImGui::GetBackgroundDrawList();
	auto window_data = get_current_manifold_lib();
	auto pos_settings = get_pos_settings();

	auto get_pos = __brush_pos();

	//draw
	dl_data->AddText(get_pos, __colorstyle(__style_color::text), __char);

	add_element(CalcTextSize(__char));
}
void gui::	checkbox(const char* __char, bool* __bool) {

	auto dl_data = ImGui::GetBackgroundDrawList();
	auto window_data = get_current_manifold_lib();
	auto pos_settings = get_pos_settings();
	//config

	auto get_pos = __brush_pos();

	//size of rect
	const float frame_size = 16.f * gui_scale;
	auto const hovered = is_hovered(get_pos, get_pos + pos(frame_size + CalcTextSize(__char).x + __style(item_spacing_x), frame_size));

	ImVec2 text_size = ImGui::CalcTextSize(__char);
	ImVec2 text_pos = get_pos + pos(frame_size + __style(item_spacing_x), (frame_size - text_size.y) / 2);
	dl_data->AddText(text_pos, *__bool ? __colorstyle(__style_color::text) : __colorstyle(__style_color::permissible_text), __char);

	float alpha;
	ImColor active_color(__colorstyle(__style_color::active_obj));
	float animated_alpha = lerp_animate(__char, "activated_frame", *__bool, int(active_color.Value.w * 255), 10, DYNAMIC);
	{
		//default
		dl_data->AddRectFilled(get_pos + pos(0, 1), get_pos + pos(frame_size, frame_size), get_frame_color(hovered));
		//active
		dl_data->AddRectFilled(get_pos + pos(0, 1), get_pos + pos(frame_size, frame_size), ImColor(active_color.Value.x, active_color.Value.y, active_color.Value.z, float(animated_alpha / 5)));
	}
	//outline
	dl_data->AddRect(get_pos, get_pos + pos(frame_size, frame_size), __colorstyle(__style_color::element_outline));

	//combat
	if (is_clicked_once() && hovered && this->is_able_to_click)
		*__bool = !(*__bool);

	const float pad = ImMax(5.0f, IM_FLOOR((frame_size / 4.0f))) * (animated_alpha / 200.f);
	RenderCheckMark(dl_data, get_pos + ImVec2(pad, pad), ImColor(255, 255, 255, int(animated_alpha)), (frame_size - (pad * 2.0f)));


	add_element(pos(0, frame_size));
}
void gui::	slider_float(const char* title, float* val, float maximal) {

	auto dl_data = ImGui::GetBackgroundDrawList();
	auto window_data = get_current_manifold_lib();
	auto pos_settings = get_pos_settings();
	auto get_pos = __brush_pos();

	//size of rect
	const float h = 10 * gui_scale, w = calc_frame_w();
	//draw
	dl_data->AddText(get_pos, __colorstyle(__style_color::text), title);

	const pos frame_draw_pos = pos(get_pos.x, get_pos.y + CalcTextSize(title).y + __style(item_spacing_y));
	auto const hovered = is_hovered(frame_draw_pos, frame_draw_pos + pos(w, h));

	dl_data->AddRectFilled(frame_draw_pos, frame_draw_pos + pos(w, h), get_frame_color(hovered, hovered && is_holding()));

	float grab_w = *val * (float(w) / float(maximal));
	float animated_grab_w = lerp_animate(title, "float_slider", true, grab_w, 15, INTERP);

	dl_data->AddRectFilled(frame_draw_pos, frame_draw_pos + pos(animated_grab_w, h), __colorstyle(__style_color::active_obj));

	//outline
	dl_data->AddRect(frame_draw_pos, frame_draw_pos + pos(w, h), __colorstyle(__style_color::element_outline));
	auto const moved_pos_of_prev_rect = 17 * gui_scale;
	auto const preview_w = CalcTextSize((std::stringstream{ } << std::setprecision(3) << *val).str().c_str()).x;
	auto const preview_h = CalcTextSize((std::stringstream{ } << std::setprecision(3) << *val).str().c_str()).y;
	auto const text_pos = frame_draw_pos + pos(animated_grab_w, moved_pos_of_prev_rect);

	dl_data->AddRectFilled(text_pos - pos(preview_w / 2 + 5 * gui_scale, 0), text_pos + pos(preview_w / 2 + 5 * gui_scale, preview_h + 3 * gui_scale), __colorstyle(__style_color::frame_default));
	dl_data->AddRect(text_pos - pos(preview_w / 2 + 5 * gui_scale, 0), text_pos + pos(preview_w / 2 + 5 * gui_scale, preview_h + 3 * gui_scale), __colorstyle(__style_color::element_outline));
	dl_data->AddText(text_pos - pos(preview_w / 2, 0), __colorstyle(__style_color::text), (std::stringstream{ } << std::setprecision(3) << *val).str().c_str());

	//last id 
	auto const id = get_propper_id(title);
	//get itp for checking
	static std::map<gui_id, bool> slider_way_1_map;
	auto sliderway1 = slider_way_1_map.find(id);
	// current itp
	if (sliderway1 == slider_way_1_map.end()) {
		slider_way_1_map.insert({ id, false });
		//insert perfect id
		sliderway1 = slider_way_1_map.find(id);
		// find id
	}

	static std::map<gui_id, bool> slider_way_2_map;
	auto sliderway2 = slider_way_2_map.find(id);
	// current itp
	if (sliderway2 == slider_way_2_map.end()) {
		slider_way_2_map.insert({ id, false });
		//insert perfect id
		sliderway2 = slider_way_2_map.find(id);
		// find id
	}

	//update cursor position
	GetCursorPos(&cursor);
	cursor.x = cursor.x - global_window_pos.x;
	cursor.y = cursor.y - global_window_pos.y;


	if (hovered && is_holding() && this->is_able_to_click) {
		*val = std::clamp(((cursor.x - frame_draw_pos.x) / (float(w) / float(maximal))), 0.f, float(maximal));
		sliderway1->second = true;
		window_data->lock_menu = true;
	}
	else if (is_holding() && sliderway1->second && this->is_able_to_click) {
		*val = std::clamp(((cursor.x - frame_draw_pos.x) / (float(w) / float(maximal))), 0.f, float(maximal));
		sliderway2->second = true;
	}
	else {
		sliderway1->second = false;
	}


	add_element(pos(0, h + CalcTextSize(title).y + __style(item_spacing_y) + moved_pos_of_prev_rect / 1.1));

}
void gui::	slider_int(const char* title, int* val, int maximal) {

	auto dl_data = ImGui::GetBackgroundDrawList();
	auto window_data = get_current_manifold_lib();
	auto pos_settings = get_pos_settings();
	auto get_pos = __brush_pos();

	//size of rect
	const float h = 10 * gui_scale, w = calc_frame_w();
	//draw
	dl_data->AddText(get_pos, __colorstyle(__style_color::text), title);

	const pos frame_draw_pos = pos(get_pos.x, get_pos.y + CalcTextSize(title).y + __style(item_spacing_y));
	auto const hovered = is_hovered(frame_draw_pos, frame_draw_pos + pos(w, h));

	dl_data->AddRectFilled(frame_draw_pos, frame_draw_pos + pos(w, h), get_frame_color(hovered, hovered && is_holding()));

	float grab_w = *val * (float(w) / float(maximal));
	float animated_grab_w = lerp_animate(title, "float_slider", true, grab_w, 15, INTERP);

	dl_data->AddRectFilled(frame_draw_pos, frame_draw_pos + pos(animated_grab_w, h), __colorstyle(__style_color::active_obj));

	//outline
	dl_data->AddRect(frame_draw_pos, frame_draw_pos + pos(w, h), __colorstyle(__style_color::element_outline));
	auto const moved_pos_of_prev_rect = 17 * gui_scale;
	auto const preview_w = CalcTextSize((std::stringstream{ } << std::setprecision(3) << *val).str().c_str()).x;
	auto const preview_h = CalcTextSize((std::stringstream{ } << std::setprecision(3) << *val).str().c_str()).y;
	auto const text_pos = frame_draw_pos + pos(animated_grab_w, moved_pos_of_prev_rect);
	dl_data->AddRectFilled(text_pos - pos(preview_w / 2 + 5 * gui_scale, 0), text_pos + pos(preview_w / 2 + 5 * gui_scale, preview_h + 3 * gui_scale), __colorstyle(__style_color::frame_default));
	dl_data->AddRect(text_pos - pos(preview_w / 2 + 5 * gui_scale, 0), text_pos + pos(preview_w / 2 + 5 * gui_scale, preview_h + 3 * gui_scale), __colorstyle(__style_color::element_outline));
	dl_data->AddText(text_pos - pos(preview_w / 2, 0), __colorstyle(__style_color::text), (std::stringstream{ } << std::setprecision(3) << *val).str().c_str());

	//last id 
	auto const id = get_propper_id(title);
	//get itp for checking
	static std::map<gui_id, bool> slider_way_1_map;
	auto sliderway1 = slider_way_1_map.find(id);
	// current itp
	if (sliderway1 == slider_way_1_map.end()) {
		slider_way_1_map.insert({ id, false });
		//insert perfect id
		sliderway1 = slider_way_1_map.find(id);
		// find id
	}

	static std::map<gui_id, bool> slider_way_2_map;
	auto sliderway2 = slider_way_2_map.find(id);
	// current itp
	if (sliderway2 == slider_way_2_map.end()) {
		slider_way_2_map.insert({ id, false });
		//insert perfect id
		sliderway2 = slider_way_2_map.find(id);
		// find id
	}

	//update cursor position
	GetCursorPos(&cursor);
	cursor.x = cursor.x - global_window_pos.x;
	cursor.y = cursor.y - global_window_pos.y;

	if (hovered && is_holding() && this->is_able_to_click) {
		*val = std::clamp(((cursor.x - frame_draw_pos.x) / (float(w) / float(maximal))), 0.f, float(maximal));
		sliderway1->second = true;
		window_data->lock_menu = true;
	}
	else if (is_holding() && sliderway1->second && this->is_able_to_click) {
		*val = std::clamp(((cursor.x - frame_draw_pos.x) / (float(w) / float(maximal))), 0.f, float(maximal));
		sliderway2->second = true;
	}
	else {
		sliderway1->second = false;
	}


	add_element(pos(0, h + CalcTextSize(title).y + __style(item_spacing_y) + moved_pos_of_prev_rect / 1.1));

}
void gui::	combo(const char* title, int* element, const char** text_array, int arr_sz ) {

	auto dl_data = ImGui::GetBackgroundDrawList();
	auto window_data = get_current_manifold_lib();
	auto pos_settings = get_pos_settings();
	auto get_pos = __brush_pos();

	//size of rect
	const float h = 20 * gui_scale, w = calc_frame_w();
	//draw
	dl_data->AddText(get_pos, __colorstyle(__style_color::text), title);

	const pos frame_draw_pos = pos(get_pos.x, get_pos.y + CalcTextSize(title).y + __style(item_spacing_y));
	auto const hovered = is_hovered(frame_draw_pos, frame_draw_pos + pos(w, h), this->is_able_to_click);
	auto const prew_size = CalcTextSize(text_array[*element]);
	dl_data->AddRectFilled(frame_draw_pos, frame_draw_pos + pos(w, h), get_frame_color(hovered, hovered && is_holding()));
	dl_data->AddRect(frame_draw_pos, frame_draw_pos + pos(w, h), __colorstyle(__style_color::element_outline));
	dl_data->AddText(frame_draw_pos + pos(4.5, h / 2 - 1 - prew_size.y / 2), __colorstyle(__style_color::text), text_array [*element] );
	dl_data->AddTriangleFilled(frame_draw_pos + pos(w - 15, 7), frame_draw_pos + pos(w - 5, 7), frame_draw_pos + pos(w - 10, 15), ImColor(50, 50, 50));

	auto const id = get_propper_id(title);
	//get itp for checking
	static std::map<gui_id, bool> is_opened_map;
	auto is_opened = is_opened_map.find(id);
	// current itp
	if (is_opened == is_opened_map.end()) {
		is_opened_map.insert({ id, false });
		//insert perfect id
		is_opened = is_opened_map.find(id);
		// find id
	}

	//open/close popup combat
	if (hovered && is_clicked_once() && this->is_able_to_click)
		is_opened->second = !is_opened->second;

	if (is_opened->second) {

		this->is_able_to_click = false;

		auto overlay = GetForegroundDrawList();
		//draw
		auto offset_element {5};
		auto popup_pos = frame_draw_pos + pos(0, h);

		if (!is_hovered(popup_pos, popup_pos + pos(w, h * arr_sz)) && is_holding()) {
			if (!hovered)
				is_opened->second = false;
		}

		//overlay->AddRectFilled(popup_pos, popup_pos + pos(w, h * arr_sz), __colorstyle(__style_color::frame_default));

		for (int i = 0; i < arr_sz; i++) {
			auto const frame_rect_now = pos(popup_pos + pos(0, h * i));
			auto const hovered { is_hovered(frame_rect_now, frame_rect_now + pos(w, h + offset_element)) };
			auto const current_char = text_array[i];
			auto const calc_current_char = CalcTextSize(current_char);

			auto const selectable_color = *element == i ? __colorstyle(__style_color::selectable_active) : __colorstyle(__style_color::selectable_default);

			overlay->AddRectFilled(frame_rect_now, frame_rect_now + pos(w , h), selectable_color);

			if (hovered)
				overlay->AddRectFilled(frame_rect_now, frame_rect_now + pos(w , h), __colorstyle(__style_color::selectable_hovered));

			if (*element == i)
			{
				overlay->AddRectFilled(frame_rect_now, frame_rect_now + pos(w , h), __colorstyle(__style_color::selectable_active));
				overlay->AddText(frame_rect_now + pos(4.5 * gui_scale, (h / 2) - calc_current_char.y / 1.85), __colorstyle(__style_color::window_background), current_char);
			}
			else
				overlay->AddText(frame_rect_now + pos(4.5 * gui_scale, (h / 2) - calc_current_char.y / 1.85), __colorstyle(__style_color::text), current_char);

			//combat
			if (hovered && is_clicked_once()) {
				*element = i;
				is_opened->second = false;
			}
		}
		overlay->AddRect(popup_pos, popup_pos + pos(w, h * arr_sz), __colorstyle(__style_color::element_outline));

	}


	add_element(pos(0, h + CalcTextSize(title).y + __style(item_spacing_y)));


}
void gui::	multi(const char* title, bool elements[], const char** text_array, int arr_sz) {

	auto dl_data = ImGui::GetBackgroundDrawList();
	auto window_data = get_current_manifold_lib();
	auto pos_settings = get_pos_settings();
	auto get_pos = __brush_pos();

	std::string preview_text;
	for (int i = 0; i < arr_sz; i++) {
		//prew 
		if (elements[i]) {
			preview_text += text_array[i];
			preview_text += ", ";
		}
	}

	if (preview_text.length() > 20) {
		preview_text.erase(20, (preview_text.length() - 20));
		preview_text.append("...");
	}


	//size of rect
	const float h = 20 * gui_scale, w = calc_frame_w();
	//draw
	dl_data->AddText(get_pos, __colorstyle(__style_color::text), title);

	const pos frame_draw_pos = pos(get_pos.x, get_pos.y + CalcTextSize(title).y + __style(item_spacing_y));
	auto const hovered = is_hovered(frame_draw_pos, frame_draw_pos + pos(w, h));
	auto const prew_size = CalcTextSize(preview_text.c_str());
	dl_data->AddRectFilled(frame_draw_pos, frame_draw_pos + pos(w, h), get_frame_color(hovered, hovered && is_holding()));
	dl_data->AddRect(frame_draw_pos, frame_draw_pos + pos(w, h), __colorstyle(__style_color::element_outline));
	dl_data->AddText(frame_draw_pos + pos(4.5, h / 2 - 1 - prew_size.y / 2), __colorstyle(__style_color::text), preview_text.c_str() );
	dl_data->AddTriangleFilled(frame_draw_pos + pos(w - 15, 7), frame_draw_pos + pos(w - 5, 7), frame_draw_pos + pos(w - 10, 15), ImColor(50, 50, 50));

	auto const id = get_propper_id(title);
	//get itp for checking
	static std::map<gui_id, bool> is_opened_map;
	auto is_opened = is_opened_map.find(id);
	// current itp
	if (is_opened == is_opened_map.end()) {
		is_opened_map.insert({ id, false });
		//insert perfect id
		is_opened = is_opened_map.find(id);
		// find id
	}

	//open/close popup combat
	if (hovered && is_clicked_once() && this->is_able_to_click)
		is_opened->second = !is_opened->second;

	if (is_opened->second) {

		this->is_able_to_click = false;

		auto overlay = GetForegroundDrawList();
		//draw
		auto offset_element{ 5 };
		auto popup_pos = frame_draw_pos + pos(0, h);

		if (!is_hovered(popup_pos, popup_pos + pos(w, h * arr_sz)) && is_holding()) {
			if (!hovered)
				is_opened->second = false;
		}

		overlay->AddRectFilled(popup_pos, popup_pos + pos(w, h * arr_sz), __colorstyle(__style_color::frame_default));

		for (int i = 0; i < arr_sz; i++) {

			auto const frame_rect_now = pos(popup_pos + pos(0, h * i));
			auto const hovered{ is_hovered(frame_rect_now, frame_rect_now + pos(w, h)) };
			auto const current_char = text_array[i];
			auto const calc_current_char = CalcTextSize(current_char);
			auto const selectable_color = elements[i] ? __colorstyle(__style_color::selectable_active) :  __colorstyle(__style_color::selectable_default);
			
			overlay->AddRectFilled(frame_rect_now, frame_rect_now + pos(w, h), selectable_color);

			if (hovered)
				overlay->AddRectFilled(frame_rect_now, frame_rect_now + pos(w, h), __colorstyle(__style_color::selectable_hovered));

			if (elements[i])
			{
				overlay->AddRectFilled(frame_rect_now, frame_rect_now + pos(1, h), __colorstyle(__style_color::window_background));
				overlay->AddText(frame_rect_now + pos(4.5, (h / 2) - calc_current_char.y / 1.85), (__colorstyle(__style_color::window_background)), current_char);
			}
			else
				overlay->AddText(frame_rect_now + pos(4.5, (h / 2) - calc_current_char.y / 1.85), __colorstyle(__style_color::text), current_char);
			//combat
			if (hovered && is_clicked_once()) 
				elements[i] = !elements[i];
		}
		overlay->AddRect(popup_pos, popup_pos + pos(w, h * arr_sz), __colorstyle(__style_color::element_outline));

	}


	add_element(pos(0, h + CalcTextSize(title).y + __style(item_spacing_y)));


}
bool gui::	button(const char* label, size ssize) {

	//tmp
	bool p = false;

	auto dl_data = ImGui::GetBackgroundDrawList();
	auto window_data = get_current_manifold_lib();
	auto pos_settings = get_pos_settings();
	auto get_pos = __brush_pos();

	//combat
	size frame_size { ssize };
	if (ssize.x == 0 && ssize.y == 0) {
		frame_size = size(calc_frame_w(), CalcTextSize(label).y * 1.5);
	}
	//draw


	auto const hovered = is_hovered(get_pos, get_pos + frame_size);

	if (hovered && is_clicked_once() && this->is_able_to_click)
		p = true;

	dl_data->AddRectFilled(get_pos, get_pos + frame_size, get_frame_color(hovered, hovered && is_holding() && this->is_able_to_click));
	dl_data->AddRect(get_pos, get_pos + frame_size, __colorstyle(__style_color::element_outline));
	dl_data->AddText(get_pos + pos((frame_size.x / 2) - CalcTextSize(label).x / 2, frame_size.y / 2) - pos(0, CalcTextSize(label).y / 2), __colorstyle(__style_color::text), label);
	
	dl_data->PushClipRect(get_pos, get_pos + frame_size);

	auto ID = ImGui::GetID((std::stringstream{} << label << "start_animation_timer").str().c_str());

	static std::map<ImGuiID, bool> pValue;
	auto ItPLibrary = pValue.find(ID);

	if (ItPLibrary == pValue.end())
	{
		pValue.insert({ ID, false });
		ItPLibrary = pValue.find(ID);
	}


	if (p)
	{
		ItPLibrary->second = true;
	}

	float circle_animation = lerp_animate(label, "circle_animate", ItPLibrary->second, frame_size.x * 2, 18, DYNAMIC);
	if (circle_animation > frame_size.x * 1.5)
	{
		ItPLibrary->second = false;
	}

	ImColor color = ImColor(__colorstyle(active_obj).Value.x, __colorstyle(active_obj).Value.y, __colorstyle(active_obj).Value.z, __colorstyle(active_obj).Value.w * (circle_animation / frame_size.x * 1));
	ImColor textcolor = ImColor(__colorstyle(__style_color::window_background).Value.x, __colorstyle(__style_color::window_background).Value.y, __colorstyle(window_background).Value.z, __colorstyle(window_background).Value.w * (circle_animation / frame_size.x * 1));

	if (circle_animation > 0.1)
		dl_data->AddCircleFilled(get_pos + size(frame_size.x / 2, frame_size.y / 2), circle_animation, color );

	if (circle_animation > 0.1)
		dl_data->AddText(get_pos + pos((frame_size.x / 2) - CalcTextSize(label).x / 2, frame_size.y / 2) - pos(0, CalcTextSize(label).y / 2), textcolor, label);

	dl_data->PopClipRect ();



	add_element(frame_size);

	return p;
}
void gui::	custom_slider(const char* title, float* val, float maximal, pos p) {

	auto dl_data = ImGui::GetForegroundDrawList();
	auto window_data = get_current_manifold_lib();
	auto pos_settings = get_pos_settings();
	auto get_pos = p;

	//size of rect
	const float h = 154 * gui_scale, w = 10 * gui_scale;
	//draw

	const pos frame_draw_pos = pos(get_pos.x, get_pos.y + CalcTextSize(title).y + __style(item_spacing_y));
	auto const hovered = is_hovered(frame_draw_pos, frame_draw_pos + pos(w, h));

	std::array<ImColor, 8> grad_array 
	{ 
		ImColor(255, 0, 0),
		ImColor(255, 0, 255),
		ImColor(20, 0, 255),
		ImColor(0, 232, 255),
		ImColor(0, 255, 10),
		ImColor(246, 255, 0),
		ImColor(255, 119, 0),
		ImColor(255, 0, 0),
	};

	if (std::string(title).find("hue_slider_cp") != std::string(title).npos) {

		for (auto i = 0; i < 7; i++) {
			//draw
			dl_data->AddRectFilledMultiColor(frame_draw_pos + pos(0, (h / 7) * i), frame_draw_pos + pos(0, (h / 7) * i) + pos(w, h / 7),
				ImColor(grad_array[7 - i]), ImColor(grad_array[7 - i]),
				ImColor(grad_array[7 - (i + 1)]), ImColor(grad_array[7 - (i + 1)]));
		}
	}
	else if (std::string(title).find("alpha_slider_cp") != std::string(title).npos) {
		//draw
		dl_data->AddRectFilledMultiColor(frame_draw_pos, frame_draw_pos + pos(w, h),
			ImColor(255, 255, 255, 0), ImColor(255, 255, 255, 0),
			ImColor(255, 255, 255, 255),   ImColor(255, 255, 255, 255));
	}

	float grab_h = *val * (float(h) / float(maximal));

	dl_data->AddRectFilled(frame_draw_pos + pos(0, grab_h - 2) - pos(1, 1), frame_draw_pos + pos(w, grab_h + 2) + pos(1, 1), ImColor(255, 255, 255), 10.f);
	dl_data->AddRectFilled(frame_draw_pos + pos(0, grab_h - 2), frame_draw_pos + pos(w, grab_h + 2), ImColor(0, 0, 0), 10.f);

	//outline
	dl_data->AddRect(frame_draw_pos, frame_draw_pos + pos(w, h), __colorstyle(__style_color::element_outline));
	auto const moved_pos_of_prev_rect = 3;
	auto const preview_w = CalcTextSize((std::stringstream{ } << std::setprecision(3) << *val).str().c_str()).x;
	auto const text_pos = frame_draw_pos + pos(grab_h, moved_pos_of_prev_rect);

	//last id 
	auto const id = get_propper_id(title);
	//get itp for checking
	static std::map<gui_id, bool> slider_way_1_map;
	auto sliderway1 = slider_way_1_map.find(id);
	// current itp
	if (sliderway1 == slider_way_1_map.end()) {
		slider_way_1_map.insert({ id, false });
		//insert perfect id
		sliderway1 = slider_way_1_map.find(id);
		// find id
	}

	static std::map<gui_id, bool> slider_way_2_map;
	auto sliderway2 = slider_way_2_map.find(id);
	// current itp
	if (sliderway2 == slider_way_2_map.end()) {
		slider_way_2_map.insert({ id, false });
		//insert perfect id
		sliderway2 = slider_way_2_map.find(id);
		// find id
	}

	//update cursor position
	GetCursorPos(&cursor);
	cursor.x = cursor.x - global_window_pos.x;
	cursor.y = cursor.y - global_window_pos.y;

	if (hovered && is_holding()) {
		*val = std::clamp(((cursor.y - frame_draw_pos.y) / (float(h) / float(maximal))), 0.f, float(maximal));
		sliderway1->second = true;
		window_data->lock_menu = true;
	}
	else if (is_holding() && sliderway1->second) {
		*val = std::clamp(((cursor.y - frame_draw_pos.y) / (float(h) / float(maximal))), 0.f, float(maximal));
		sliderway2->second = true;
	}
	else {
		sliderway1->second = false;
	}

}
void gui::	color_picker(const char* title, float col[4]) {

	auto dl_data = ImGui::GetBackgroundDrawList();
	auto window_data = get_current_manifold_lib();
	auto pos_settings = get_pos_settings();
	auto overlay = GetForegroundDrawList();
	ImColor pickercolor = ImColor(col[0], col[1], col[2], col[3]);
	//config
	auto get_pos = __brush_pos();
	auto const transparent_background = true;
	//size of rect
	const float frame_size = 15.f * gui_scale;
	auto const hovered = is_hovered(get_pos, get_pos + pos(frame_size + CalcTextSize(title).x + __style(item_spacing_x), frame_size));
	//draw
	ImVec2 text_size = CalcTextSize(title);
	ImVec2 text_pos = get_pos + pos(frame_size + __style(item_spacing_x), (frame_size - text_size.y) / 2);

	dl_data->AddText(text_pos,  __colorstyle(__style_color::text), title);

	if (transparent_background) {

		dl_data->AddRectFilled(get_pos, get_pos + pos(frame_size, frame_size), ImColor(255, 255, 255, 255));
		{
			dl_data->AddRectFilled(get_pos + pos(0, 0), get_pos + pos(0, 0) + pos(5 * gui_scale, 5 * gui_scale), ImColor(0, 0, 0, 100));
			dl_data->AddRectFilled(get_pos + pos(10 * gui_scale, 0), get_pos + pos(10 * gui_scale, 0) + pos(5 * gui_scale, 5 * gui_scale), ImColor(0, 0, 0, 100));
			//middle
			dl_data->AddRectFilled(get_pos + pos(5 * gui_scale, 5 * gui_scale), get_pos + pos(5 * gui_scale, 5 * gui_scale) + pos(5 * gui_scale, 5 * gui_scale), ImColor(0, 0, 0, 100));

			dl_data->AddRectFilled(get_pos + pos(0, 10 * gui_scale), get_pos + pos(0, 10 * gui_scale) + pos(5 * gui_scale, 5 * gui_scale), ImColor(0, 0, 0, 100));
			dl_data->AddRectFilled(get_pos + pos(10 * gui_scale, 10 * gui_scale), get_pos + pos(10 * gui_scale, 10 * gui_scale) + pos(5 * gui_scale, 5 * gui_scale), ImColor(0, 0, 0, 100));
		}

	}

	//frame
	dl_data->AddRectFilled(get_pos, get_pos + pos(frame_size, frame_size), ImColor(col[0], col[1], col[2], col[3]) );
	//outline
	dl_data->AddRect(get_pos, get_pos + pos(frame_size, frame_size), __colorstyle(__style_color::element_outline));

	auto const id = get_propper_id(title);
	//get itp for checking
	static std::map<gui_id, bool> is_opened_map;
	auto is_opened = is_opened_map.find(id);
	// current itp
	if (is_opened == is_opened_map.end()) {
		is_opened_map.insert({ id, false });
		//insert perfect id
		is_opened = is_opened_map.find(id);
		// find id
	}

	if (is_clicked_once() && hovered && this->is_able_to_click)
		is_opened->second = !is_opened->second;

	if (true) {

		//get itp for checking
		static std::map<gui_id, float> slider_way_2_map_color;
		auto sliderway2_color = slider_way_2_map_color.find(id);

		static std::map<gui_id, float> slider_way_1_map_color;
		auto sliderway1_color = slider_way_1_map_color.find(id);

		// current itp
		if (sliderway1_color == slider_way_1_map_color.end()) {
			slider_way_1_map_color.insert({ id, 0.0f });
			//insert perfect id
			sliderway1_color = slider_way_1_map_color.find(id);
			// find id
		}

		// current itp
		if (sliderway2_color == slider_way_2_map_color.end()) {
			slider_way_2_map_color.insert({ id, 0.0f });
			//insert perfect id
			sliderway2_color = slider_way_2_map_color.find(id);
			// find id
		}

		if (is_opened->second) {

			this->is_able_to_click = false;

			static std::map<gui_id, float> old_hue_map;
			auto old_hue = old_hue_map.find(id);

			// current itp
			if (old_hue == old_hue_map.end()) {
				old_hue_map.insert({ id, 0.0f });
				//insert perfect id
				old_hue = old_hue_map.find(id);
				// find id
			}

			auto popup_pos = get_pos + pos(30 * gui_scale, 30 * gui_scale);
			auto const popup_size = size((310 * gui_scale / 1.5) * gui_scale, (270 * gui_scale / 1.5) * gui_scale);

			//frame
			overlay->AddRectFilled(popup_pos, popup_pos + popup_size, __colorstyle(__style_color::frame_default));
			//outline
			overlay->AddRect(popup_pos, popup_pos + popup_size, __colorstyle(__style_color::element_outline));


			//grad
			auto const inside_pos = popup_pos + pos(8 * gui_scale, 8 * gui_scale);
			auto const inside_size = popup_size - pos(8 * gui_scale, 8 * gui_scale);

			int off_bars_distance = 35 * gui_scale;
			pos end_of_multibar = inside_pos + pos(inside_size.x - 8 * gui_scale - off_bars_distance, 0 * gui_scale);

			//set H
			custom_slider((std::stringstream{} << "hue_slider_cp" << title).str().c_str(), &sliderway1_color->second, 100, end_of_multibar + pos(7 * gui_scale, -15 * gui_scale));
			//set alpha
			custom_slider((std::stringstream{} << "alpha_slider_cp" << title).str().c_str(), &sliderway2_color->second, 255, end_of_multibar + pos(23 * gui_scale, -15 * gui_scale));


			ImColor grad[4];
			ImColor updated;

			const float H = sliderway1_color->second / 100.f;

			grad[0].SetHSV(H, 0, 1);	
			grad[1].SetHSV(H, 0, 100);	
			grad[2].SetHSV(H, 1, 200);	
			grad[3].SetHSV(H, 1, 0);	

			updated.Value.x = grad[2].Value.x  / 255.f;
			updated.Value.y = grad[2].Value.y  / 255.f;
			updated.Value.z = grad[2].Value.z  / 255.f;
			updated.Value.w = 1.f;

			//draw
			overlay->AddRectFilledMultiColor(inside_pos, inside_pos + pos(inside_size.x - 8 * gui_scale - off_bars_distance, inside_size.y - 8 * gui_scale), grad[0], grad[1], updated, grad[3]);
			overlay->AddRect(inside_pos, inside_pos + pos(inside_size.x - 8 * gui_scale - off_bars_distance, inside_size.y - 8 * gui_scale), __colorstyle(__style_color::element_outline));

			//picker :::

			bool read = false;

			static pos old_picker_pos;
			pos real_picker_pos = pos(0, 0);

			if (is_hovered(inside_pos, inside_pos + pos(inside_size.x - 8 * gui_scale - off_bars_distance, inside_size.y - 8 * gui_scale))) {

				if (is_holding()) {

					read = true;

					//get oldest screen dc
					HDC screenDC = GetDC(menu::get().hWnd);

					//get bit, use descriptor memory DC
					//BitBlt(memDC, 0, 0, 2, 2, screenDC, cursor.x - 1, cursor.y - 1, SRCCOPY);

					//get color, that we got from the bitblt origin

					//change
					GetCursorPos(&cursor);
					cursor.x = cursor.x - global_window_pos.x;
					cursor.y = cursor.y - global_window_pos.y;

					auto picked_color_from_screen = GetPixel(screenDC, cursor.x, cursor.y);
					
					//convert COLORREF ()
					auto
					red_picked   = GetRValue(picked_color_from_screen),
					green_picked = GetGValue(picked_color_from_screen),
					blue_picked  = GetBValue(picked_color_from_screen);
					//https://docs.microsoft.com/en-us/windows/win32/gdi/colorref

					/*
					rgbRed   =  0x000000FF;  rgbGreen =  0x0000FF00; 
					rgbBlue  =  0x00FF0000;  rgbBlack =  0x00000000; 
					rgbWhite =  0x00FFFFFF;
					*/

					//update color if imgui element, using colorref converted data
					pickercolor.Value.x = red_picked / 255.f; 
					//just a bit lose, bit ok
					pickercolor.Value.y = green_picked / 255.f;
					pickercolor.Value.z = blue_picked / 255.f;
					
					old_picker_pos = pos(cursor.x - inside_pos.x, cursor.y - inside_pos.y);

			
				}

			}

			real_picker_pos = inside_pos + old_picker_pos;

			if (!read) {
				overlay->AddCircle(real_picker_pos, 3.f * gui_scale, ImColor(0, 0, 0), 3 * 15.f);
				overlay->AddCircle(real_picker_pos, 5.f * gui_scale, ImColor(255, 255, 255), 5 * 15.f);
			}

			read = FALSE;

			//draw
		}
		//apply
		col[3] = sliderway2_color->second / 255.f;
		col[0] = pickercolor.Value.x;
		col[1] = pickercolor.Value.y;
		col[2] = pickercolor.Value.z;
	}


	add_element(pos(0, frame_size));
}
void gui::	list(const char* title, int* element, const char* text[], int count) {

	auto dl_data = ImGui::GetBackgroundDrawList();
	auto window_data = get_current_manifold_lib();
	auto pos_settings = get_pos_settings();
	auto get_pos = __brush_pos();
	auto const h = 20.f * gui_scale;
	auto const w = calc_frame_w();
	auto const frame_pos  = pos(get_pos.x, get_pos.y + CalcTextSize(title).y + __style(__style_value::item_spacing_y));
	auto const frame_size = size(w, (count + 1) * h);
	dl_data->AddRectFilled(frame_pos, frame_pos + frame_size, __colorstyle(window_background));
	dl_data->AddRect(frame_pos, frame_pos + frame_size, __colorstyle(__style_color::window_oultine));
	dl_data->AddText(get_pos, __colorstyle(__style_color::text), title);
	for (int i = 0; i < count; i++) {
		auto const frame_rect_now = pos(frame_pos + pos(1, h * i + 1));
		auto const hovered { is_hovered(frame_rect_now, frame_rect_now + pos(w, h)) };
		auto const current_char = text[i];
		auto const calc_current_char = CalcTextSize(current_char);
		auto const selectable_color = *element == i ? __colorstyle(__style_color::selectable_active) : __colorstyle(__style_color::selectable_default);

		dl_data->AddRectFilled(frame_rect_now, frame_rect_now + pos(w - 2 * gui_scale, h), selectable_color);

		if (hovered)
			dl_data->AddRectFilled(frame_rect_now, frame_rect_now + pos(w - 2 * gui_scale, h), __colorstyle(__style_color::selectable_hovered));

		if (*element == i) 
		{
			dl_data->AddRectFilled(frame_rect_now, frame_rect_now + pos(1 * gui_scale, h), __colorstyle(__style_color::active_obj));
			dl_data->AddText(frame_rect_now + pos(4.5 * gui_scale, (h / 2) - calc_current_char.y / 1.85), __colorstyle(__style_color::window_background), current_char);
		}
		else
			dl_data->AddText(frame_rect_now + pos(4.5 * gui_scale, (h / 2) - calc_current_char.y / 1.85), __colorstyle(__style_color::text), current_char);
		//combat
		if (hovered && is_clicked_once() && this->is_able_to_click)
			*element = i;
	}
	add_element(frame_size + pos(0, h));

}
void gui::	multilist(const char* title, bool element[], const char* text[], int count) {

	auto dl_data = ImGui::GetBackgroundDrawList();
	auto window_data = get_current_manifold_lib();
	auto pos_settings = get_pos_settings();
	auto get_pos = __brush_pos();
	auto const h = 20.f * gui_scale;
	auto const w = calc_frame_w();
	auto const frame_pos = pos(get_pos.x, get_pos.y + CalcTextSize(title).y + __style(__style_value::item_spacing_y));
	auto const frame_size = size(w, (count + 1) * h);
	dl_data->AddRectFilled(frame_pos, frame_pos + frame_size, __colorstyle(window_background));
	dl_data->AddRect(frame_pos, frame_pos + frame_size, __colorstyle(__style_color::window_oultine));
	dl_data->AddText(get_pos, __colorstyle(__style_color::text), title);
	for (int i = 0; i < count; i++) 
	{
		auto const frame_rect_now = pos(frame_pos + pos(1, h * i + 1));
		auto const hovered{ is_hovered(frame_rect_now, frame_rect_now + pos(w, h)) };
		auto const current_char = text[i];
		auto const calc_current_char = CalcTextSize(current_char);
		auto const selectable_color = element[i] == true ? __colorstyle(__style_color::selectable_active) : __colorstyle(__style_color::selectable_default);

		dl_data->AddRectFilled(frame_rect_now, frame_rect_now + pos(w - 2 * gui_scale, h), selectable_color);

		if (hovered)
			dl_data->AddRectFilled(frame_rect_now, frame_rect_now + pos(w - 2 * gui_scale, h), __colorstyle(__style_color::selectable_hovered));

		if (element[i] == true)
		{
			dl_data->AddRectFilled(frame_rect_now, frame_rect_now + pos(1 * gui_scale, h), __colorstyle(__style_color::active_obj));
			dl_data->AddText(frame_rect_now + pos(4.5 * gui_scale, (h / 2) - calc_current_char.y / 1.85), __colorstyle(__style_color::window_background), current_char);
		}
		else
			dl_data->AddText(frame_rect_now + pos(4.5 * gui_scale, (h / 2) - calc_current_char.y / 1.85), __colorstyle(__style_color::text), current_char);
		//combat
		if (hovered && is_clicked_once() && this->is_able_to_click)
			element[i] = !element[i];
	}
	add_element(frame_size + pos(0, h));

}
void gui::	input(const char* title, std::string& input) {

	auto dl_data = ImGui::GetBackgroundDrawList();
	auto window_data = get_current_manifold_lib();
	auto const pos_settings = get_pos_settings();
	auto const get_pos = __brush_pos();
	const auto w = calc_frame_w();
	const auto h = CalcTextSize(title).y * 1.5;
	auto const frame_pos = pos(get_pos.x, get_pos.y + CalcTextSize(title).y +__style(__style_value::item_spacing_y));
	auto const frame_size = size(w, h);
	auto const hovered = is_hovered(frame_pos, frame_pos + frame_size);

	auto const id = get_propper_id(title);
	//get itp for checking
	static std::map<gui_id, bool> is_opened_map;
	//animation
	static std::map<gui_id, int> blicking_map;
	//anim switch
	static std::map<gui_id, int> blicking_map_second;
	auto is_ready_to_input = is_opened_map.find(id);
	// current itp
	if (is_ready_to_input == is_opened_map.end()) {
		is_opened_map.insert({ id, false });
		//insert perfect id
		is_ready_to_input = is_opened_map.find(id);
		// find id
	}

	auto blicking = blicking_map.find(id);
	// current itp
	if (blicking == blicking_map.end()) {
		blicking_map.insert({ id, 0.0f });
		//insert perfect id
		blicking = blicking_map.find(id);
		// find id
	}

	auto switch_anim = blicking_map_second.find(id);
	// current itp
	if (switch_anim == blicking_map_second.end()) {
		blicking_map_second.insert({ id, 0.0f });
		//insert perfect id
		switch_anim = blicking_map_second.find(id);
		// find id
	}

	//set active id
	if (hovered && is_clicked_once() && this->is_able_to_click)
		is_ready_to_input->second = true;
	else if (!hovered && is_clicked_once() && this->is_able_to_click)
		is_ready_to_input->second = false;

	blicking->second++;
	if (blicking->second % 50 == 0)
		switch_anim->second = !switch_anim->second;

	//uniform
	dl_data->AddText(get_pos, __colorstyle(__style_color::text), title);
	dl_data->AddRectFilled(frame_pos, frame_pos + frame_size, get_frame_color(hovered, is_ready_to_input->second));
	dl_data->AddRect(frame_pos, frame_pos + frame_size, __colorstyle(__style_color::element_outline));
	//pos
	pos converted_cursor_pos;
	converted_cursor_pos = frame_pos + pos(5 + CalcTextSize(input.c_str()).x + 1, 4);
	//draw cursor
	if (is_ready_to_input->second)
		dl_data->AddRect(converted_cursor_pos, converted_cursor_pos + size(1, h - 7), ImColor(255, 255, 255, switch_anim->second ? 255 : 0));

	//check keys state
	for (int i = 0; i < 255; i++) {
		//check input validation
		if ((i > 47 && i < 91) || i == 190) {
			const bool valid  = CalcTextSize(input.c_str()).x < w - 8;
			const bool upcase = GetKeyState(VK_CAPITAL) || GetAsyncKeyState(VK_SHIFT);
			//delete last char
			if (GetAsyncKeyState(VK_BACK) & 1 && input.length() > 0) {
				//check, if can we try to edit text
				if (is_ready_to_input->second) 
					input.erase(input.length() - 1, 1);
			}
			else if (GetAsyncKeyState(189) & 1) {
				if (is_ready_to_input->second && valid) {
					if (upcase) input += "_";
					else		input += "-";
				}
			}
			else if (GetAsyncKeyState(190) & 1) {
				if (is_ready_to_input->second && valid)
					input += ".";
			}
			//do skip
			else if (GetAsyncKeyState(VK_SPACE) & 1) {
				if (is_ready_to_input->second && valid)
				input += " ";
			}
			//get click state
			else if (GetAsyncKeyState(i) & 1) {
				std::string tmp = key_names[i];
				//check capslock activity, shift
				if (upcase) { 
					if (i >= 48 && i <= 57) 
						tmp = other_key_names[i - 48];
					else std::transform(tmp.begin(), tmp.end(), tmp.begin(), change_case); 
				} 
				//add
				if (is_ready_to_input->second && valid)
					input += tmp;
			}
			
		}
	}


	dl_data->AddText(frame_pos + pos(4.5, h / 2 - 1 - CalcTextSize(input.c_str()).y / 2), __colorstyle(__style_color::text), input.c_str());

	add_element(frame_size + pos(0, CalcTextSize(title).y + __style(__style_value::item_spacing_y)));
}
void gui::	text_box(const char* title, const char* text[], int count) {

	auto dl_data = ImGui::GetBackgroundDrawList();
	auto window_data = get_current_manifold_lib();
	auto const pos_settings = get_pos_settings();
	auto const get_pos = __brush_pos();
	const auto w = calc_frame_w();
	const auto h = ((CalcTextSize(text[0]).y) * count) + (4.5 * 2);
	auto const frame_pos = pos(get_pos.x, get_pos.y + CalcTextSize(title).y + __style(__style_value::item_spacing_y));
	auto const frame_size = size(w, h);
	auto const hovered = is_hovered(frame_pos, frame_pos + frame_size);

	//uniform
	dl_data->AddText(get_pos, __colorstyle(__style_color::text), title);
	dl_data->AddRectFilled(frame_pos, frame_pos + frame_size, __colorstyle(__style_color::frame_default));
	dl_data->AddRect(frame_pos, frame_pos + frame_size, __colorstyle(__style_color::element_outline));

	for (auto i = 0; i < count; i++)
		dl_data->AddText(frame_pos + pos(4.5, 4.5 + (CalcTextSize(text[i]).y * i)), __colorstyle(__style_color::text), text[i]);

	add_element(frame_size + pos(0, CalcTextSize(title).y + __style(__style_value::item_spacing_y)));
}
bool gui::	toggle_button(const char* label, bool* v, size ssize) {

	//tmp
	bool p = false;

	auto dl_data = ImGui::GetBackgroundDrawList();
	auto window_data = get_current_manifold_lib();
	auto pos_settings = get_pos_settings();
	auto get_pos = __brush_pos();

	//combat
	size frame_size{ ssize };
	if (ssize.x == 0 && ssize.y == 0) 
		frame_size = size(calc_frame_w(), CalcTextSize(label).y * 2);
	
	//draw
	auto const hovered = is_hovered(get_pos, get_pos + frame_size);

	if (hovered && is_clicked_once() && this->is_able_to_click) 
	{
		p = true; 
		*v = !(*v);
	}


	float active_alpha = lerp_animate(label, "toogle_button_alpha_lerp", *v, __colorstyle(__style_color::active_obj).Value.w * 255, 15, DYNAMIC);
	dl_data->AddRect(get_pos, get_pos + frame_size, __colorstyle(__style_color::unactive_obj));

	auto activated_color = ImColor(__colorstyle(__style_color::active_obj).Value.x, __colorstyle(__style_color::active_obj).Value.y, __colorstyle(__style_color::active_obj).Value.z, active_alpha / 255);

	dl_data->AddRect(get_pos, get_pos + frame_size, activated_color);
	dl_data->AddRectFilled(get_pos, get_pos + size(frame_size.x * (active_alpha / (__colorstyle(__style_color::active_obj).Value.w * 255)), frame_size.y), __colorstyle(__style_color::active_obj));

	dl_data->AddText(get_pos + pos((frame_size.x / 2) - CalcTextSize(label).x / 2, frame_size.y / 2) - pos(0, CalcTextSize(label).y / 2), *v ? __colorstyle(__style_color::window_background) : __colorstyle(__style_color::permissible_text), label);


	add_element(frame_size);

	return p;
}
void gui::	hotkey(const char* title, int* key, int* mode) {

	auto dl_data = ImGui::GetBackgroundDrawList();
	auto window_data = get_current_manifold_lib();
	auto pos_settings = get_pos_settings();
	auto get_pos = __brush_pos();
	auto overlay = GetForegroundDrawList();

	//size of rect
	const float h = 20 * gui_scale, w = calc_frame_w();
	//draw
	dl_data->AddText(get_pos, __colorstyle(__style_color::text), title);

	const pos frame_draw_pos = pos(get_pos.x, get_pos.y + CalcTextSize(title).y + __style(item_spacing_y));
	auto const hovered = is_hovered(frame_draw_pos, frame_draw_pos + pos(w, h));
	auto const prew_size = CalcTextSize(title);
	dl_data->AddRectFilled(frame_draw_pos, frame_draw_pos + pos(w, h), get_frame_color(hovered, hovered && is_holding()));
	dl_data->AddRect(frame_draw_pos, frame_draw_pos + pos(w, h), __colorstyle(__style_color::element_outline));

	auto const id = get_propper_id(title);
	//get itp for checking
	static std::map<gui_id, bool> is_opened_map_hotkey;
	//animation
	auto is_ready_to_input22 = is_opened_map_hotkey.find(id);
	// current itp
	if (is_ready_to_input22 == is_opened_map_hotkey.end()) {
		is_opened_map_hotkey.insert({ id, false });
		//insert perfect id
		is_ready_to_input22 = is_opened_map_hotkey.find(id);
		// find id
	}

	static std::map<gui_id, bool> opened_popup_map_hotkey;
	//animation
	auto is_opened_popup_hotkey = opened_popup_map_hotkey.find(id);
	// current itp
	if (is_opened_popup_hotkey == opened_popup_map_hotkey.end()) {
		opened_popup_map_hotkey.insert({ id, false });
		//insert perfect id
		is_opened_popup_hotkey = opened_popup_map_hotkey.find(id);
		// find id
	}

	static std::map<gui_id, pos> cursor_map;
	//animation
	auto cursor_updated = cursor_map.find(id);
	// current itp
	if (cursor_updated == cursor_map.end()) {
		cursor_map.insert({ id, pos(cursor.x, cursor.y)});
		//insert perfect id
		cursor_updated = cursor_map.find(id);
		// find id
	}

	//get itp for checking
	static std::map<gui_id, std::string> tmp_str;
	//animation
	auto tmp_str_input = tmp_str.find(id);
	// current itp
	if (tmp_str_input == tmp_str.end()) {
		tmp_str.insert({ id, "None" });
		//insert perfect id
		tmp_str_input = tmp_str.find(id);
		// find id
	}

	std::string tmp_key_str = "None";

	//active
	if (hovered && is_clicked_once() && this->is_able_to_click) {
		is_ready_to_input22->second = !is_ready_to_input22->second;
		//set input
	}
	//is_opened_popup
	if (hovered && is_clicked_once(1) && this->is_able_to_click) {
		//right click
		cursor_updated->second = pos(cursor.x, cursor.y);
		is_opened_popup_hotkey->second = !is_opened_popup_hotkey->second;
	}

	for (int i = 0; i < 166; i++) {
		//check all states
		int key_state = GetAsyncKeyState(i);
		if (is_ready_to_input22->second) {
			//ready
			if (key_state && !is_holding() && !is_holding(1)) {
				//set new one key 
				*key = i;
				tmp_str_input->second = key_names[i];
				//set new one txt
				is_ready_to_input22->second = false;
			}
		}
	}

	dl_data->AddText(frame_draw_pos + pos(4.5, h / 2 - 1 - prew_size.y / 2), is_ready_to_input22->second ? __colorstyle(__style_color::active_obj) : __colorstyle(__style_color::text), tmp_str_input->second.c_str());

	add_element(size(w, h + CalcTextSize(title).y + __style(item_spacing_y)));

	const char* modes[]{ "Toggle", "Hold" };
	auto const  oldpopuppos = frame_draw_pos + pos(10, 10);
	pos popup_pos = frame_draw_pos + pos(cursor_updated->second.x - frame_draw_pos.x, cursor_updated->second.y - frame_draw_pos.y);
	auto const  popup_size = size((CalcTextSize(modes[0]).x + __style(item_spacing_x)) * 1.1, (CalcTextSize(modes[0]).y + CalcTextSize(modes[1]).y + __style(item_spacing_y)) * 1.1);

	if (is_opened_popup_hotkey->second) {

		this->is_able_to_click = false;
		//create selection popup
		overlay->AddRectFilled(popup_pos, popup_pos + popup_size, ImColor(__colorstyle(__style_color::frame_default).Value.x, __colorstyle(__style_color::frame_default).Value.y, __colorstyle(__style_color::frame_default).Value.z, 0.75f));
		overlay->AddRect(popup_pos, popup_pos + popup_size, __colorstyle(__style_color::element_outline));
		//text
		overlay->AddText(popup_pos + pos(__style(item_spacing_x) / 2, __style(item_spacing_y) / 2), *mode == 0 ? __colorstyle(__style_color::active_obj) : __colorstyle(__style_color::text), modes[0]);
		overlay->AddText(popup_pos + pos(__style(item_spacing_x) / 2, __style(item_spacing_y) / 2 + CalcTextSize(modes[0]).y + 2), *mode == 1 ? __colorstyle(__style_color::active_obj) : __colorstyle(__style_color::text), modes[1]);
		//mode 1
		const bool hovered_mode1 = is_hovered(popup_pos + pos(__style(item_spacing_x) / 2,
			__style(item_spacing_y) / 2), popup_pos + pos(__style(item_spacing_x) / 2, __style(item_spacing_y) / 2) + pos(CalcTextSize(modes[0]).x + 2, CalcTextSize(modes[0]).y + 2));
		//mode 2
		const bool hovered_mode2 = is_hovered(popup_pos + pos(__style(item_spacing_x) / 2, __style(item_spacing_y) / 2 + CalcTextSize(modes[0]).y + 2),
			popup_pos + pos(__style(item_spacing_x) / 2, __style(item_spacing_y) / 2 + CalcTextSize(modes[1]).y + 2) + pos(CalcTextSize(modes[1]).x + 2, CalcTextSize(modes[1]).y + 2));

		if (hovered_mode1 && is_clicked_once()) {
			*mode = 0;
			is_opened_popup_hotkey->second = false;
		}
		else if (hovered_mode2 && is_clicked_once()) {
			*mode = 1;
			is_opened_popup_hotkey->second = false;
		}
	}

	if (!hovered && is_holding(0) && !is_hovered(popup_pos, popup_pos + popup_size)) {
		is_opened_popup_hotkey->second = false;
	}
}
/*
////////////////////////////////////////////////////////////////////
*/