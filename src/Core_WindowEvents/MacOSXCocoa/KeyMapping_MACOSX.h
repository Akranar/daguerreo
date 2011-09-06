#ifndef _KEY_MAPPING_MACOSX_H_
#define _KEY_MAPPING_MACOSX_H_

/*
    From:
        -> /System/Library/Frameworks/Carbon.framework/Versions/A/Frameworks/HIToolbox.framework/Versions/A/Headers/Events.h
*/

enum {
    kVK_ANSI_A                    = 0x00,
    kVK_ANSI_S                    = 0x01,
    kVK_ANSI_D                    = 0x02,
    kVK_ANSI_F                    = 0x03,
    kVK_ANSI_H                    = 0x04,
    kVK_ANSI_G                    = 0x05,
    kVK_ANSI_Z                    = 0x06,
    kVK_ANSI_X                    = 0x07,
    kVK_ANSI_C                    = 0x08,
    kVK_ANSI_V                    = 0x09,
    kVK_ANSI_B                    = 0x0B,
    kVK_ANSI_Q                    = 0x0C,
    kVK_ANSI_W                    = 0x0D,
    kVK_ANSI_E                    = 0x0E,
    kVK_ANSI_R                    = 0x0F,
    kVK_ANSI_Y                    = 0x10,
    kVK_ANSI_T                    = 0x11,
    kVK_ANSI_1                    = 0x12,
    kVK_ANSI_2                    = 0x13,
    kVK_ANSI_3                    = 0x14,
    kVK_ANSI_4                    = 0x15,
    kVK_ANSI_6                    = 0x16,
    kVK_ANSI_5                    = 0x17,
    kVK_ANSI_Equal                = 0x18,
    kVK_ANSI_9                    = 0x19,
    kVK_ANSI_7                    = 0x1A,
    kVK_ANSI_Minus                = 0x1B,
    kVK_ANSI_8                    = 0x1C,
    kVK_ANSI_0                    = 0x1D,
    kVK_ANSI_RightBracket         = 0x1E,
    kVK_ANSI_O                    = 0x1F,
    kVK_ANSI_U                    = 0x20,
    kVK_ANSI_LeftBracket          = 0x21,
    kVK_ANSI_I                    = 0x22,
    kVK_ANSI_P                    = 0x23,
    kVK_ANSI_L                    = 0x25,
    kVK_ANSI_J                    = 0x26,
    kVK_ANSI_Quote                = 0x27,
    kVK_ANSI_K                    = 0x28,
    kVK_ANSI_Semicolon            = 0x29,
    kVK_ANSI_Backslash            = 0x2A,
    kVK_ANSI_Comma                = 0x2B,
    kVK_ANSI_Slash                = 0x2C,
    kVK_ANSI_N                    = 0x2D,
    kVK_ANSI_M                    = 0x2E,
    kVK_ANSI_Period               = 0x2F,
    kVK_ANSI_Grave                = 0x32,
    kVK_ANSI_KeypadDecimal        = 0x41,
    kVK_ANSI_KeypadMultiply       = 0x43,
    kVK_ANSI_KeypadPlus           = 0x45,
    kVK_ANSI_KeypadClear          = 0x47,
    kVK_ANSI_KeypadDivide         = 0x4B,
    kVK_ANSI_KeypadEnter          = 0x4C,
    kVK_ANSI_KeypadMinus          = 0x4E,
    kVK_ANSI_KeypadEquals         = 0x51,
    kVK_ANSI_Keypad0              = 0x52,
    kVK_ANSI_Keypad1              = 0x53,
    kVK_ANSI_Keypad2              = 0x54,
    kVK_ANSI_Keypad3              = 0x55,
    kVK_ANSI_Keypad4              = 0x56,
    kVK_ANSI_Keypad5              = 0x57,
    kVK_ANSI_Keypad6              = 0x58,
    kVK_ANSI_Keypad7              = 0x59,
    kVK_ANSI_Keypad8              = 0x5B,
    kVK_ANSI_Keypad9              = 0x5C
};

/* keycodes for keys that are independent of keyboard layout*/
enum {
    kVK_Return                    = 0x24,
    kVK_Tab                       = 0x30,
    kVK_Space                     = 0x31,
    kVK_Delete                    = 0x33,
    kVK_Escape                    = 0x35,
	kVK_RightCommand			  = 0x36, //Added by Akranar 5/6/2011
    kVK_Command                   = 0x37,
    kVK_Shift                     = 0x38,
    kVK_CapsLock                  = 0x39,
    kVK_Option                    = 0x3A,
    kVK_Control                   = 0x3B,
    kVK_RightShift                = 0x3C,
    kVK_RightOption               = 0x3D,
    kVK_RightControl              = 0x3E,
    kVK_Function                  = 0x3F,
    kVK_F17                       = 0x40,
    kVK_VolumeUp                  = 0x48,
    kVK_VolumeDown                = 0x49,
    kVK_Mute                      = 0x4A,
    kVK_F18                       = 0x4F,
    kVK_F19                       = 0x50,
    kVK_F20                       = 0x5A,
    kVK_F5                        = 0x60,
    kVK_F6                        = 0x61,
    kVK_F7                        = 0x62,
    kVK_F3                        = 0x63,
    kVK_F8                        = 0x64,
    kVK_F9                        = 0x65,
    kVK_F11                       = 0x67,
    kVK_F13                       = 0x69,
    kVK_F16                       = 0x6A,
    kVK_F14                       = 0x6B,
    kVK_F10                       = 0x6D,
	kVK_Menu					  = 0x6E, //Added by Akranar 5/6/2011
    kVK_F12                       = 0x6F,
    kVK_F15                       = 0x71,
    kVK_Help                      = 0x72,
    kVK_Home                      = 0x73,
    kVK_PageUp                    = 0x74,
    kVK_ForwardDelete             = 0x75,
    kVK_F4                        = 0x76,
    kVK_End                       = 0x77,
    kVK_F2                        = 0x78,
    kVK_PageDown                  = 0x79,
    kVK_F1                        = 0x7A,
    kVK_LeftArrow                 = 0x7B,
    kVK_RightArrow                = 0x7C,
    kVK_DownArrow                 = 0x7D,
    kVK_UpArrow                   = 0x7E
};

/* ISO keyboards only*/
enum {
    kVK_ISO_Section               = 0x0A
};

/* JIS keyboards only*/
enum {
    kVK_JIS_Yen                   = 0x5D,
    kVK_JIS_Underscore            = 0x5E,
    kVK_JIS_KeypadComma           = 0x5F,
    kVK_JIS_Eisu                  = 0x66,
    kVK_JIS_Kana                  = 0x68
};


static KeyID::Key KeyMap[256];

void InitKeyMap()
{
	
	for (unsigned int i = 0; i < 256; ++i)
	{
		KeyMap[i] = KeyID::KEY_UNKNOWN;
	}

	KeyMap[kVK_ANSI_0] = KeyID::KEY_0;
	KeyMap[kVK_ANSI_1] = KeyID::KEY_1;
	KeyMap[kVK_ANSI_2] = KeyID::KEY_2;
	KeyMap[kVK_ANSI_3] = KeyID::KEY_3;
	KeyMap[kVK_ANSI_4] = KeyID::KEY_4;
	KeyMap[kVK_ANSI_5] = KeyID::KEY_5;
	KeyMap[kVK_ANSI_6] = KeyID::KEY_6;
	KeyMap[kVK_ANSI_7] = KeyID::KEY_7;
	KeyMap[kVK_ANSI_8] = KeyID::KEY_8;
	KeyMap[kVK_ANSI_9] = KeyID::KEY_9;

	KeyMap[kVK_ANSI_A] = KeyID::KEY_A;
	KeyMap[kVK_ANSI_B] = KeyID::KEY_B;
	KeyMap[kVK_ANSI_C] = KeyID::KEY_C;
	KeyMap[kVK_ANSI_D] = KeyID::KEY_D;
	KeyMap[kVK_ANSI_E] = KeyID::KEY_E;
	KeyMap[kVK_ANSI_F] = KeyID::KEY_F;
	KeyMap[kVK_ANSI_G] = KeyID::KEY_G;
	KeyMap[kVK_ANSI_H] = KeyID::KEY_H;
	KeyMap[kVK_ANSI_I] = KeyID::KEY_I;
	KeyMap[kVK_ANSI_J] = KeyID::KEY_J;
	KeyMap[kVK_ANSI_K] = KeyID::KEY_K;
	KeyMap[kVK_ANSI_L] = KeyID::KEY_L;
	KeyMap[kVK_ANSI_M] = KeyID::KEY_M;
	KeyMap[kVK_ANSI_N] = KeyID::KEY_N;
	KeyMap[kVK_ANSI_O] = KeyID::KEY_O;
	KeyMap[kVK_ANSI_P] = KeyID::KEY_P;
	KeyMap[kVK_ANSI_Q] = KeyID::KEY_Q;
	KeyMap[kVK_ANSI_R] = KeyID::KEY_R;
	KeyMap[kVK_ANSI_S] = KeyID::KEY_S;
	KeyMap[kVK_ANSI_T] = KeyID::KEY_T;
	KeyMap[kVK_ANSI_U] = KeyID::KEY_U;
	KeyMap[kVK_ANSI_V] = KeyID::KEY_V;
	KeyMap[kVK_ANSI_W] = KeyID::KEY_W;
	KeyMap[kVK_ANSI_X] = KeyID::KEY_X;
	KeyMap[kVK_ANSI_Y] = KeyID::KEY_Y;
	KeyMap[kVK_ANSI_Z] = KeyID::KEY_Z;

	KeyMap[kVK_Control] = KeyID::KEY_LEFT_CTRL;
	KeyMap[kVK_Option] = KeyID::KEY_LEFT_ALT;
	KeyMap[kVK_Shift] = KeyID::KEY_LEFT_SHIFT;
	KeyMap[kVK_Command] = KeyID::KEY_LEFT_SUPER;

	KeyMap[kVK_RightControl] = KeyID::KEY_RIGHT_CTRL;
	KeyMap[kVK_RightOption] = KeyID::KEY_RIGHT_ALT;
	KeyMap[kVK_RightShift] = KeyID::KEY_RIGHT_SHIFT;
	KeyMap[kVK_RightCommand] = KeyID::KEY_RIGHT_SUPER;

	KeyMap[kVK_Menu] = KeyID::KEY_MENU;

	KeyMap[kVK_Space] = KeyID::KEY_SPACE;
	KeyMap[kVK_ANSI_Grave] = KeyID::KEY_TILDE;
	KeyMap[kVK_Tab] = KeyID::KEY_TAB;
	KeyMap[kVK_CapsLock] = KeyID::KEY_CAPSLOCK;

	KeyMap[kVK_ANSI_Minus] = KeyID::KEY_UNDERSCORE;
	KeyMap[kVK_ANSI_Equal] = KeyID::KEY_EQUAL;
	KeyMap[kVK_Delete] = KeyID::KEY_BACKSPACE;
	KeyMap[kVK_ANSI_LeftBracket] = KeyID::KEY_OPEN_BRACKET;
	KeyMap[kVK_ANSI_RightBracket] = KeyID::KEY_CLOSE_BRACKET;
	KeyMap[kVK_ANSI_Backslash] = KeyID::KEY_BACK_SLASH;
	KeyMap[kVK_ANSI_Semicolon] = KeyID::KEY_SEMICOLON;
	KeyMap[kVK_ANSI_Quote] = KeyID::KEY_APOSTROPHE;
	KeyMap[kVK_ANSI_Comma] = KeyID::KEY_COMMA;
	KeyMap[kVK_ANSI_Period] = KeyID::KEY_PERIOD;
	KeyMap[kVK_ANSI_Slash] = KeyID::KEY_FORWARD_SLASH;
	KeyMap[kVK_Return] = KeyID::KEY_RETURN;

	KeyMap[kVK_Escape] = KeyID::KEY_ESCAPE;
	KeyMap[kVK_F1] = KeyID::KEY_F1;
	KeyMap[kVK_F2] = KeyID::KEY_F2;
	KeyMap[kVK_F3] = KeyID::KEY_F3;
	KeyMap[kVK_F4] = KeyID::KEY_F4;
	KeyMap[kVK_F5] = KeyID::KEY_F5;
	KeyMap[kVK_F6] = KeyID::KEY_F6;
	KeyMap[kVK_F7] = KeyID::KEY_F7;
	KeyMap[kVK_F8] = KeyID::KEY_F8;
	KeyMap[kVK_F9] = KeyID::KEY_F9;
	KeyMap[kVK_F10] = KeyID::KEY_F10;
	KeyMap[kVK_F11] = KeyID::KEY_F11;
	KeyMap[kVK_F12] = KeyID::KEY_F12;
	KeyMap[kVK_F13] = KeyID::KEY_F13;
	KeyMap[kVK_F14] = KeyID::KEY_F14;
	KeyMap[kVK_F15] = KeyID::KEY_F15;

	//KeyMap[VK_NUMLOCK] = KeyID::KEY_NUMLOCK;
	//No numlock on mac?

	KeyMap[kVK_ANSI_Keypad0] = KeyID::KEY_NUM_0;
	KeyMap[kVK_ANSI_Keypad1] = KeyID::KEY_NUM_1;
	KeyMap[kVK_ANSI_Keypad2] = KeyID::KEY_NUM_2;
	KeyMap[kVK_ANSI_Keypad3] = KeyID::KEY_NUM_3;
	KeyMap[kVK_ANSI_Keypad4] = KeyID::KEY_NUM_4;
	KeyMap[kVK_ANSI_Keypad5] = KeyID::KEY_NUM_5;
	KeyMap[kVK_ANSI_Keypad6] = KeyID::KEY_NUM_6;
	KeyMap[kVK_ANSI_Keypad7] = KeyID::KEY_NUM_7;
	KeyMap[kVK_ANSI_Keypad8] = KeyID::KEY_NUM_8;
	KeyMap[kVK_ANSI_Keypad9] = KeyID::KEY_NUM_9;
	KeyMap[kVK_ANSI_KeypadDecimal] = KeyID::KEY_NUM_PERIOD;

	KeyMap[kVK_End] = KeyID::KEY_END;
	KeyMap[kVK_Home] = KeyID::KEY_HOME;
	KeyMap[kVK_PageDown] = KeyID::KEY_PAGE_DOWN;
	KeyMap[kVK_PageUp] = KeyID::KEY_PAGE_UP;
	KeyMap[kVK_ANSI_KeypadClear] = KeyID::KEY_CLEAR;

	KeyMap[kVK_Help] = KeyID::KEY_INSERT; //
	KeyMap[kVK_ForwardDelete] = KeyID::KEY_DELETE;

	KeyMap[kVK_UpArrow] = KeyID::KEY_UP;
	KeyMap[kVK_DownArrow] = KeyID::KEY_DOWN;
	KeyMap[kVK_LeftArrow] = KeyID::KEY_LEFT;
	KeyMap[kVK_RightArrow] = KeyID::KEY_RIGHT;

	//These keys not available.
	/*
	KeyMap[VK_SNAPSHOT] = KeyID::KEY_PRINT;
	KeyMap[VK_SCROLL] = KeyID::KEY_SCROLL_LOCK;
	KeyMap[VK_PAUSE] = KeyID::KEY_PAUSE;
	*/

	KeyMap[kVK_ANSI_KeypadEquals] = KeyID::KEY_NUM_EQUAL;
	KeyMap[kVK_ANSI_KeypadDivide] = KeyID::KEY_NUM_DIVIDE;
	KeyMap[kVK_ANSI_KeypadPlus] = KeyID::KEY_NUM_ADD;
	KeyMap[kVK_ANSI_KeypadMinus] = KeyID::KEY_NUM_SUBTRACT;
	KeyMap[kVK_ANSI_KeypadMultiply] = KeyID::KEY_NUM_MULTIPLY;

	KeyMap[kVK_ANSI_KeypadEnter] = KeyID::KEY_NUM_RETURN;
}


#endif