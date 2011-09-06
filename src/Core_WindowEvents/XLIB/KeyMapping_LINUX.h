#ifndef _KEY_MAPPING_LINUX_H_
#define _KEY_MAPPING_LINUX_H_

#include "../KeyID.h"
#include <X11/keysym.h>

static KeyID::Key KeyMap_LayoutIndep[256];
static KeyID::Key KeyMap_LayoutDep[256];

void InitKeyMap()
{
	
	for (unsigned int i = 0; i < 256; ++i)
	{
		KeyMap_LayoutIndep[i] = KeyID::KEY_UNKNOWN;
	}
	for (unsigned int i = 0; i < 256; ++i)
	{
		KeyMap_LayoutDep[i] = KeyID::KEY_UNKNOWN;
	}

	KeyMap_LayoutDep[XK_0] = KeyID::KEY_0;
	KeyMap_LayoutDep[XK_1] = KeyID::KEY_1;
	KeyMap_LayoutDep[XK_2] = KeyID::KEY_2;
	KeyMap_LayoutDep[XK_3] = KeyID::KEY_3;
	KeyMap_LayoutDep[XK_4] = KeyID::KEY_4;
	KeyMap_LayoutDep[XK_5] = KeyID::KEY_5;
	KeyMap_LayoutDep[XK_6] = KeyID::KEY_6;
	KeyMap_LayoutDep[XK_7] = KeyID::KEY_7;
	KeyMap_LayoutDep[XK_8] = KeyID::KEY_8;
	KeyMap_LayoutDep[XK_9] = KeyID::KEY_9;

	KeyMap_LayoutDep[XK_a] = KeyID::KEY_A;
	KeyMap_LayoutDep[XK_b] = KeyID::KEY_B;
	KeyMap_LayoutDep[XK_c] = KeyID::KEY_C;
	KeyMap_LayoutDep[XK_d] = KeyID::KEY_D;
	KeyMap_LayoutDep[XK_e] = KeyID::KEY_E;
	KeyMap_LayoutDep[XK_f] = KeyID::KEY_F;
	KeyMap_LayoutDep[XK_g] = KeyID::KEY_G;
	KeyMap_LayoutDep[XK_h] = KeyID::KEY_H;
	KeyMap_LayoutDep[XK_i] = KeyID::KEY_I;
	KeyMap_LayoutDep[XK_j] = KeyID::KEY_J;
	KeyMap_LayoutDep[XK_k] = KeyID::KEY_K;
	KeyMap_LayoutDep[XK_l] = KeyID::KEY_L;
	KeyMap_LayoutDep[XK_m] = KeyID::KEY_M;
	KeyMap_LayoutDep[XK_n] = KeyID::KEY_N;
	KeyMap_LayoutDep[XK_o] = KeyID::KEY_O;
	KeyMap_LayoutDep[XK_p] = KeyID::KEY_P;
	KeyMap_LayoutDep[XK_q] = KeyID::KEY_Q;
	KeyMap_LayoutDep[XK_r] = KeyID::KEY_R;
	KeyMap_LayoutDep[XK_s] = KeyID::KEY_S;
	KeyMap_LayoutDep[XK_t] = KeyID::KEY_T;
	KeyMap_LayoutDep[XK_u] = KeyID::KEY_U;
	KeyMap_LayoutDep[XK_v] = KeyID::KEY_V;
	KeyMap_LayoutDep[XK_w] = KeyID::KEY_W;
	KeyMap_LayoutDep[XK_x] = KeyID::KEY_X;
	KeyMap_LayoutDep[XK_y] = KeyID::KEY_Y;
	KeyMap_LayoutDep[XK_z] = KeyID::KEY_Z;

	KeyMap_LayoutIndep[XK_Control_L & 0xff] = KeyID::KEY_LEFT_CTRL;
	KeyMap_LayoutIndep[XK_Alt_L & 0xff] = KeyID::KEY_LEFT_ALT;
	KeyMap_LayoutIndep[XK_Shift_L & 0xff] = KeyID::KEY_LEFT_SHIFT;
	KeyMap_LayoutIndep[XK_Super_L & 0xff] = KeyID::KEY_LEFT_SUPER;

	KeyMap_LayoutIndep[XK_Control_R & 0xff] = KeyID::KEY_RIGHT_CTRL;
	KeyMap_LayoutIndep[XK_Alt_R & 0xff] = KeyID::KEY_RIGHT_ALT;
	KeyMap_LayoutIndep[XK_Shift_R & 0xff] = KeyID::KEY_RIGHT_SHIFT;
	KeyMap_LayoutIndep[XK_Super_R & 0xff] = KeyID::KEY_RIGHT_SUPER;

	KeyMap_LayoutIndep[XK_Hyper_R & 0xff] = KeyID::KEY_MENU;

	KeyMap_LayoutDep[XK_space] = KeyID::KEY_SPACE;
	KeyMap_LayoutDep[XK_grave] = KeyID::KEY_TILDE;
	KeyMap_LayoutIndep[XK_Tab & 0xff] = KeyID::KEY_TAB;
	KeyMap_LayoutIndep[XK_Caps_Lock & 0xff] = KeyID::KEY_CAPSLOCK;

	KeyMap_LayoutDep[XK_minus] = KeyID::KEY_UNDERSCORE;
	KeyMap_LayoutDep[XK_equal] = KeyID::KEY_EQUAL;
	KeyMap_LayoutIndep[XK_BackSpace & 0xff] = KeyID::KEY_BACKSPACE;
	KeyMap_LayoutDep[XK_bracketleft] = KeyID::KEY_OPEN_BRACKET;
	KeyMap_LayoutDep[XK_bracketright] = KeyID::KEY_CLOSE_BRACKET;
	KeyMap_LayoutDep[XK_backslash] = KeyID::KEY_BACK_SLASH;
	KeyMap_LayoutDep[XK_semicolon] = KeyID::KEY_SEMICOLON;
	KeyMap_LayoutDep[XK_quoteright] = KeyID::KEY_APOSTROPHE;
	KeyMap_LayoutDep[XK_comma] = KeyID::KEY_COMMA;
	KeyMap_LayoutDep[XK_period] = KeyID::KEY_PERIOD;
	KeyMap_LayoutDep[XK_slash] = KeyID::KEY_FORWARD_SLASH;
	KeyMap_LayoutIndep[XK_Return & 0xff] = KeyID::KEY_RETURN;

	KeyMap_LayoutIndep[XK_Escape & 0xff] = KeyID::KEY_ESCAPE;
	KeyMap_LayoutIndep[XK_F1 & 0xff] = KeyID::KEY_F1;
	KeyMap_LayoutIndep[XK_F2 & 0xff] = KeyID::KEY_F2;
	KeyMap_LayoutIndep[XK_F3 & 0xff] = KeyID::KEY_F3;
	KeyMap_LayoutIndep[XK_F4 & 0xff] = KeyID::KEY_F4;
	KeyMap_LayoutIndep[XK_F5 & 0xff] = KeyID::KEY_F5;
	KeyMap_LayoutIndep[XK_F6 & 0xff] = KeyID::KEY_F6;
	KeyMap_LayoutIndep[XK_F7 & 0xff] = KeyID::KEY_F7;
	KeyMap_LayoutIndep[XK_F8 & 0xff] = KeyID::KEY_F8;
	KeyMap_LayoutIndep[XK_F9 & 0xff] = KeyID::KEY_F9;
	KeyMap_LayoutIndep[XK_F10 & 0xff] = KeyID::KEY_F10;
	KeyMap_LayoutIndep[XK_F11 & 0xff] = KeyID::KEY_F11;
	KeyMap_LayoutIndep[XK_F12 & 0xff] = KeyID::KEY_F12;
	KeyMap_LayoutIndep[XK_F13 & 0xff] = KeyID::KEY_F13;
	KeyMap_LayoutIndep[XK_F14 & 0xff] = KeyID::KEY_F14;
	KeyMap_LayoutIndep[XK_F15 & 0xff] = KeyID::KEY_F15;

	KeyMap_LayoutIndep[XK_Num_Lock & 0xff] = KeyID::KEY_NUMLOCK;

	KeyMap_LayoutDep[XK_KP_0 & 0xff] = KeyID::KEY_NUM_0;
	KeyMap_LayoutDep[XK_KP_1 & 0xff] = KeyID::KEY_NUM_1;
	KeyMap_LayoutDep[XK_KP_2 & 0xff] = KeyID::KEY_NUM_2;
	KeyMap_LayoutDep[XK_KP_3 & 0xff] = KeyID::KEY_NUM_3;
	KeyMap_LayoutDep[XK_KP_4 & 0xff] = KeyID::KEY_NUM_4;
	KeyMap_LayoutDep[XK_KP_5 & 0xff] = KeyID::KEY_NUM_5;
	KeyMap_LayoutDep[XK_KP_6 & 0xff] = KeyID::KEY_NUM_6;
	KeyMap_LayoutDep[XK_KP_7 & 0xff] = KeyID::KEY_NUM_7;
	KeyMap_LayoutDep[XK_KP_8 & 0xff] = KeyID::KEY_NUM_8;
	KeyMap_LayoutDep[XK_KP_9 & 0xff] = KeyID::KEY_NUM_9;
	KeyMap_LayoutDep[XK_KP_Decimal & 0xff] = KeyID::KEY_NUM_PERIOD;

	KeyMap_LayoutIndep[XK_End & 0xff] = KeyID::KEY_END;
	KeyMap_LayoutIndep[XK_Home & 0xff] = KeyID::KEY_HOME;
	KeyMap_LayoutIndep[XK_Page_Down & 0xff] = KeyID::KEY_PAGE_DOWN;
	KeyMap_LayoutIndep[XK_Page_Up & 0xff] = KeyID::KEY_PAGE_UP;
	KeyMap_LayoutIndep[XK_Clear & 0xff] = KeyID::KEY_CLEAR;

	KeyMap_LayoutIndep[XK_Insert & 0xff] = KeyID::KEY_INSERT;
	KeyMap_LayoutIndep[XK_Delete & 0xff] = KeyID::KEY_DELETE;

	KeyMap_LayoutIndep[XK_Up & 0xff] = KeyID::KEY_UP;
	KeyMap_LayoutIndep[XK_Down & 0xff] = KeyID::KEY_DOWN;
	KeyMap_LayoutIndep[XK_Left & 0xff] = KeyID::KEY_LEFT;
	KeyMap_LayoutIndep[XK_Right & 0xff] = KeyID::KEY_RIGHT;

	KeyMap_LayoutIndep[XK_Print & 0xff] = KeyID::KEY_PRINT;
	KeyMap_LayoutIndep[XK_Scroll_Lock & 0xff] = KeyID::KEY_SCROLL_LOCK;
	KeyMap_LayoutIndep[XK_Pause & 0xff] = KeyID::KEY_PAUSE;

	KeyMap_LayoutIndep[XK_KP_Equal & 0xff] = KeyID::KEY_NUM_EQUAL;
	KeyMap_LayoutIndep[XK_KP_Divide & 0xff] = KeyID::KEY_NUM_DIVIDE;
	KeyMap_LayoutIndep[XK_KP_Add & 0xff] = KeyID::KEY_NUM_ADD;
	KeyMap_LayoutIndep[XK_KP_Subtract & 0xff] = KeyID::KEY_NUM_SUBTRACT;
	KeyMap_LayoutIndep[XK_KP_Multiply & 0xff] = KeyID::KEY_NUM_MULTIPLY;

	KeyMap_LayoutIndep[XK_KP_Enter & 0xff] = KeyID::KEY_NUM_RETURN;

}

#endif
