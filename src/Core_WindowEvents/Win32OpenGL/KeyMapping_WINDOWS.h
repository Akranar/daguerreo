#ifndef _KEY_MAPPING_WINDOWS_H_
#define _KEY_MAPPING_WINDOWS_H_

#include "../KeyID.h"

#ifndef VK_0
#define VK_0	'0'
#define VK_1	'1'
#define VK_2	'2'
#define VK_3	'3'
#define VK_4	'4'
#define VK_5	'5'
#define VK_6	'6'
#define VK_7	'7'
#define VK_8	'8'
#define VK_9	'9'
#define VK_A	'A'
#define VK_B	'B'
#define VK_C	'C'
#define VK_D	'D'
#define VK_E	'E'
#define VK_F	'F'
#define VK_G	'G'
#define VK_H	'H'
#define VK_I	'I'
#define VK_J	'J'
#define VK_K	'K'
#define VK_L	'L'
#define VK_M	'M'
#define VK_N	'N'
#define VK_O	'O'
#define VK_P	'P'
#define VK_Q	'Q'
#define VK_R	'R'
#define VK_S	'S'
#define VK_T	'T'
#define VK_U	'U'
#define VK_V	'V'
#define VK_W	'W'
#define VK_X	'X'
#define VK_Y	'Y'
#define VK_Z	'Z'
#endif

static KeyID::Key KeyMap[223]; //223 because the largest value is 222 for VK_OEM_7, KEY_APOSTROPHE

void InitKeyMap()
{
	
	for (unsigned int i = 0; i < 223; ++i)
	{
		KeyMap[i] = KeyID::KEY_UNKNOWN;
	}

	KeyMap[VK_0] = KeyID::KEY_0;
	KeyMap[VK_1] = KeyID::KEY_1;
	KeyMap[VK_2] = KeyID::KEY_2;
	KeyMap[VK_3] = KeyID::KEY_3;
	KeyMap[VK_4] = KeyID::KEY_4;
	KeyMap[VK_5] = KeyID::KEY_5;
	KeyMap[VK_6] = KeyID::KEY_6;
	KeyMap[VK_7] = KeyID::KEY_7;
	KeyMap[VK_8] = KeyID::KEY_8;
	KeyMap[VK_9] = KeyID::KEY_9;

	KeyMap[VK_A] = KeyID::KEY_A;
	KeyMap[VK_B] = KeyID::KEY_B;
	KeyMap[VK_C] = KeyID::KEY_C;
	KeyMap[VK_D] = KeyID::KEY_D;
	KeyMap[VK_E] = KeyID::KEY_E;
	KeyMap[VK_F] = KeyID::KEY_F;
	KeyMap[VK_G] = KeyID::KEY_G;
	KeyMap[VK_H] = KeyID::KEY_H;
	KeyMap[VK_I] = KeyID::KEY_I;
	KeyMap[VK_J] = KeyID::KEY_J;
	KeyMap[VK_K] = KeyID::KEY_K;
	KeyMap[VK_L] = KeyID::KEY_L;
	KeyMap[VK_M] = KeyID::KEY_M;
	KeyMap[VK_N] = KeyID::KEY_N;
	KeyMap[VK_O] = KeyID::KEY_O;
	KeyMap[VK_P] = KeyID::KEY_P;
	KeyMap[VK_Q] = KeyID::KEY_Q;
	KeyMap[VK_R] = KeyID::KEY_R;
	KeyMap[VK_S] = KeyID::KEY_S;
	KeyMap[VK_T] = KeyID::KEY_T;
	KeyMap[VK_U] = KeyID::KEY_U;
	KeyMap[VK_V] = KeyID::KEY_V;
	KeyMap[VK_W] = KeyID::KEY_W;
	KeyMap[VK_X] = KeyID::KEY_X;
	KeyMap[VK_Y] = KeyID::KEY_Y;
	KeyMap[VK_Z] = KeyID::KEY_Z;

	KeyMap[VK_LCONTROL] = KeyID::KEY_LEFT_CTRL;
	KeyMap[VK_LMENU] = KeyID::KEY_LEFT_ALT;
	KeyMap[VK_LSHIFT] = KeyID::KEY_LEFT_SHIFT;
	KeyMap[VK_LWIN] = KeyID::KEY_LEFT_SUPER;

	KeyMap[VK_RCONTROL] = KeyID::KEY_RIGHT_CTRL;
	KeyMap[VK_RMENU] = KeyID::KEY_RIGHT_ALT;
	KeyMap[VK_RSHIFT] = KeyID::KEY_RIGHT_SHIFT;
	KeyMap[VK_RWIN] = KeyID::KEY_RIGHT_SUPER;

	KeyMap[VK_APPS] = KeyID::KEY_MENU;

	KeyMap[VK_SPACE] = KeyID::KEY_SPACE;
	KeyMap[VK_OEM_3] = KeyID::KEY_TILDE;
	KeyMap[VK_TAB] = KeyID::KEY_TAB;
	KeyMap[VK_CAPITAL] = KeyID::KEY_CAPSLOCK;

	KeyMap[VK_OEM_MINUS] = KeyID::KEY_UNDERSCORE;
	KeyMap[VK_OEM_PLUS] = KeyID::KEY_EQUAL;
	KeyMap[VK_BACK] = KeyID::KEY_BACKSPACE;
	KeyMap[VK_OEM_4] = KeyID::KEY_OPEN_BRACKET;
	KeyMap[VK_OEM_6] = KeyID::KEY_CLOSE_BRACKET;
	KeyMap[VK_OEM_5] = KeyID::KEY_BACK_SLASH;
	KeyMap[VK_OEM_1] = KeyID::KEY_SEMICOLON;
	KeyMap[VK_OEM_7] = KeyID::KEY_APOSTROPHE;
	KeyMap[VK_OEM_COMMA] = KeyID::KEY_COMMA;
	KeyMap[VK_OEM_PERIOD] = KeyID::KEY_PERIOD;
	KeyMap[VK_OEM_2] = KeyID::KEY_FORWARD_SLASH;
	KeyMap[VK_RETURN] = KeyID::KEY_RETURN;

	KeyMap[VK_ESCAPE] = KeyID::KEY_ESCAPE;
	KeyMap[VK_F1] = KeyID::KEY_F1;
	KeyMap[VK_F2] = KeyID::KEY_F2;
	KeyMap[VK_F3] = KeyID::KEY_F3;
	KeyMap[VK_F4] = KeyID::KEY_F4;
	KeyMap[VK_F5] = KeyID::KEY_F5;
	KeyMap[VK_F6] = KeyID::KEY_F6;
	KeyMap[VK_F7] = KeyID::KEY_F7;
	KeyMap[VK_F8] = KeyID::KEY_F8;
	KeyMap[VK_F9] = KeyID::KEY_F9;
	KeyMap[VK_F10] = KeyID::KEY_F10;
	KeyMap[VK_F11] = KeyID::KEY_F11;
	KeyMap[VK_F12] = KeyID::KEY_F12;
	KeyMap[VK_F13] = KeyID::KEY_F13;
	KeyMap[VK_F14] = KeyID::KEY_F14;
	KeyMap[VK_F15] = KeyID::KEY_F15;

	KeyMap[VK_NUMLOCK] = KeyID::KEY_NUMLOCK;

	KeyMap[VK_NUMPAD0] = KeyID::KEY_NUM_0;
	KeyMap[VK_NUMPAD1] = KeyID::KEY_NUM_1;
	KeyMap[VK_NUMPAD2] = KeyID::KEY_NUM_2;
	KeyMap[VK_NUMPAD3] = KeyID::KEY_NUM_3;
	KeyMap[VK_NUMPAD4] = KeyID::KEY_NUM_4;
	KeyMap[VK_NUMPAD5] = KeyID::KEY_NUM_5;
	KeyMap[VK_NUMPAD6] = KeyID::KEY_NUM_6;
	KeyMap[VK_NUMPAD7] = KeyID::KEY_NUM_7;
	KeyMap[VK_NUMPAD8] = KeyID::KEY_NUM_8;
	KeyMap[VK_NUMPAD9] = KeyID::KEY_NUM_9;
	KeyMap[VK_DECIMAL] = KeyID::KEY_NUM_PERIOD;

	KeyMap[VK_END] = KeyID::KEY_END;
	KeyMap[VK_HOME] = KeyID::KEY_HOME;
	KeyMap[VK_NEXT] = KeyID::KEY_PAGE_DOWN;
	KeyMap[VK_PRIOR] = KeyID::KEY_PAGE_UP;
	KeyMap[VK_CLEAR] = KeyID::KEY_CLEAR;

	KeyMap[VK_INSERT] = KeyID::KEY_INSERT;
	KeyMap[VK_DELETE] = KeyID::KEY_DELETE;

	KeyMap[VK_UP] = KeyID::KEY_UP;
	KeyMap[VK_DOWN] = KeyID::KEY_DOWN;
	KeyMap[VK_LEFT] = KeyID::KEY_LEFT;
	KeyMap[VK_RIGHT] = KeyID::KEY_RIGHT;

	KeyMap[VK_SNAPSHOT] = KeyID::KEY_PRINT;
	KeyMap[VK_SCROLL] = KeyID::KEY_SCROLL_LOCK;
	KeyMap[VK_PAUSE] = KeyID::KEY_PAUSE;

	KeyMap[VK_CLEAR] = KeyID::KEY_NUM_EQUAL;
	KeyMap[VK_DIVIDE] = KeyID::KEY_NUM_DIVIDE;
	KeyMap[VK_ADD] = KeyID::KEY_NUM_ADD;
	KeyMap[VK_SUBTRACT] = KeyID::KEY_NUM_SUBTRACT;
	KeyMap[VK_MULTIPLY] = KeyID::KEY_NUM_MULTIPLY;

	KeyMap[VK_RETURN] = KeyID::KEY_NUM_RETURN;
}

#endif