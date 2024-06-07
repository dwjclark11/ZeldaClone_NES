#pragma once

/*
*  The key numbers are defined based on SDLKey values
*  These are not the SCANCODE values, but the virtual
*  Values based on the ASCII Keycodes
*
*/
constexpr int KEY_UNKNOWN						= 	-1;
constexpr int KEY_FIRST							=  0;
constexpr int KEY_BACKSPACE						=  8;
constexpr int KEY_TAB							=  9;
constexpr int KEY_CLEAR							= 12;
constexpr int KEY_RETURN						= 13;
constexpr int KEY_PAUSE							= 19;
constexpr int KEY_ESCAPE						= 27;
constexpr int KEY_SPACE							= 32;
constexpr int KEY_EXCLAIM						= 33;
constexpr int KEY_QUOTEDBL						= 34;
constexpr int KEY_HASH							= 35;
constexpr int KEY_DOLLAR						= 36;
constexpr int KEY_AMPERSAND						= 38;
constexpr int KEY_QUOTE							= 39;
constexpr int KEY_LEFTPAREN						= 40;
constexpr int KEY_RIGHTPAREN					= 41;
constexpr int KEY_ASTERISK						= 42;
constexpr int KEY_PLUS							= 43;
constexpr int KEY_COMMA							= 44;
constexpr int KEY_MINUS							= 45;
constexpr int KEY_PERIOD						= 46;
constexpr int KEY_SLASH							= 47;
constexpr int KEY_0								= 48;
constexpr int KEY_1								= 49;
constexpr int KEY_2								= 50;
constexpr int KEY_3								= 51;
constexpr int KEY_4								= 52;
constexpr int KEY_5								= 53;
constexpr int KEY_6								= 54;
constexpr int KEY_7								= 55;
constexpr int KEY_8								= 56;
constexpr int KEY_9								= 57;
constexpr int KEY_COLON							= 58;
constexpr int KEY_SEMICOLON						= 59;
constexpr int KEY_LESS							= 60;
constexpr int KEY_EQUALS						= 61;
constexpr int KEY_GREATER						= 62;
constexpr int KEY_QUESTION						= 63;
constexpr int KEY_AT = 64;

constexpr int KEY_LEFTBRACKET					=  91;
constexpr int KEY_BACKSLASH						=  92;
constexpr int KEY_RIGHTBRACKET					=  93;
constexpr int KEY_CARET							=  94;
constexpr int KEY_UNDERSCORE					=  95;
constexpr int KEY_BACKQUOTE						=  96;
constexpr int KEY_A								=  97;
constexpr int KEY_B								=  98;
constexpr int KEY_C								=  99;
constexpr int KEY_D								= 100;
constexpr int KEY_E								= 101;
constexpr int KEY_F								= 102;
constexpr int KEY_G								= 103;
constexpr int KEY_H								= 104;
constexpr int KEY_I								= 105;
constexpr int KEY_J								= 106;
constexpr int KEY_K								= 107;
constexpr int KEY_L								= 108;
constexpr int KEY_M								= 109;
constexpr int KEY_N								= 110;
constexpr int KEY_O								= 111;
constexpr int KEY_P								= 112;
constexpr int KEY_Q								= 113;
constexpr int KEY_R								= 114;
constexpr int KEY_S								= 115;
constexpr int KEY_T								= 116;
constexpr int KEY_U								= 117;
constexpr int KEY_V								= 118;
constexpr int KEY_W								= 119;
constexpr int KEY_X								= 120;
constexpr int KEY_Y								= 121;
constexpr int KEY_Z								= 122;
constexpr int KEY_DELETE						= 127;


/*
	All the keys below are not mapped by ASCII values like the keys above.
	These numbers are converted by the function "int ConvertKeysNum(int key)"
	in Keyboard.cpp
*/

constexpr int KEY_CAPSLOCK						= 181;

constexpr int KEY_F1							= 182;
constexpr int KEY_F2							= 183;
constexpr int KEY_F3							= 184;
constexpr int KEY_F4							= 185;
constexpr int KEY_F5							= 186;
constexpr int KEY_F6							= 187;
constexpr int KEY_F7							= 188;
constexpr int KEY_F8							= 189;
constexpr int KEY_F9							= 190;
constexpr int KEY_F10							= 191;
constexpr int KEY_F11							= 192;
constexpr int KEY_F12							= 193;


constexpr int KEY_SCROLLOCK						= 195;

constexpr int KEY_INSERT						= 197;
constexpr int KEY_HOME							= 198;
constexpr int KEY_PAGEUP						= 199;
constexpr int KEY_END							= 201;
constexpr int KEY_PAGEDOWN						= 202;

constexpr int KEY_RIGHT							= 203;
constexpr int KEY_LEFT							= 204;
constexpr int KEY_DOWN							= 205;
constexpr int KEY_UP							= 206;

/*
	Keypad keys after conversion
*/
constexpr int KEY_NUMLOCK						= 207;
constexpr int KEY_KP_DIVIDE						= 208;
constexpr int KEY_KP_MULTIPLY					= 209;
constexpr int KEY_KP_MINUS						= 210;
constexpr int KEY_KP_PLUS						= 211;
constexpr int KEY_KP_ENTER						= 212;
constexpr int KEY_KP1							= 213;
constexpr int KEY_KP2							= 214;
constexpr int KEY_KP3							= 215;
constexpr int KEY_KP4							= 216;
constexpr int KEY_KP5							= 217;
constexpr int KEY_KP6							= 218;
constexpr int KEY_KP7							= 219;
constexpr int KEY_KP8							= 220;
constexpr int KEY_KP9							= 221;
constexpr int KEY_KP0							= 222;
constexpr int KEY_KP_PERIOD						= 223;


constexpr int KEY_LCTRL							= 348;
constexpr int KEY_LSHIFT						= 349;
constexpr int KEY_LALT							= 350;
constexpr int KEY_RCTRL							= 352;
constexpr int KEY_RSHIFT						= 353;
constexpr int KEY_RALT							= 354;

constexpr int KEY_LAST							= 355;
