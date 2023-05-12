#pragma once

/*
*  The key numbers are defined based on SDLKey values
*  These are not the SCANCODE values, but the virtual
*  Values based on the ASCII Keycodes
*
*/
#define KEY_UNKNOWN							0
#define KEY_FIRST                    0
#define KEY_BACKSPACE                8
#define KEY_TAB                      9
#define KEY_CLEAR                    12
#define KEY_RETURN                   13
#define KEY_PAUSE                    19
#define KEY_ESCAPE                   27
#define KEY_SPACE                    32
#define KEY_EXCLAIM                  33
#define KEY_QUOTEDBL                 34
#define KEY_HASH                     35
#define KEY_DOLLAR                   36
#define KEY_AMPERSAND                38
#define KEY_QUOTE                    39
#define KEY_LEFTPAREN                40
#define KEY_RIGHTPAREN               41
#define KEY_ASTERISK                 42
#define KEY_PLUS                     43
#define KEY_COMMA                    44
#define KEY_MINUS                    45
#define KEY_PERIOD                   46
#define KEY_SLASH                    47
#define KEY_0                        48
#define KEY_1                        49
#define KEY_2                        50
#define KEY_3                        51
#define KEY_4                        52
#define KEY_5                        53
#define KEY_6                        54
#define KEY_7                        55
#define KEY_8                        56
#define KEY_9                        57
#define KEY_COLON                    58
#define KEY_SEMICOLON                59
#define KEY_LESS                     60
#define KEY_EQUALS                   61
#define KEY_GREATER                  62
#define KEY_QUESTION                 63
#define KEY_AT                       64

#define KEY_LEFTBRACKET              91
#define KEY_BACKSLASH                92
#define KEY_RIGHTBRACKET             93
#define KEY_CARET                    94
#define KEY_UNDERSCORE               95
#define KEY_BACKQUOTE                96
#define KEY_A                        97
#define KEY_B                        98
#define KEY_C                        99
#define KEY_D                        100
#define KEY_E                        101
#define KEY_F                        102
#define KEY_G                        103
#define KEY_H                        104
#define KEY_I                        105
#define KEY_J                        106
#define KEY_K                        107
#define KEY_L                        108
#define KEY_M                        109
#define KEY_N                        110
#define KEY_O                        111
#define KEY_P                        112
#define KEY_Q                        113
#define KEY_R                        114
#define KEY_S                        115
#define KEY_T                        116
#define KEY_U                        117
#define KEY_V                        118
#define KEY_W                        119
#define KEY_X                        120
#define KEY_Y                        121
#define KEY_Z                        122
#define KEY_DELETE                   127



/*
	All the keys below are not mapped by ASCII values like the keys above.
	These numbers are converted by the function "int ConvertKeysNum(int key)"
	in Keyboard.cpp
*/

#define KEY_CAPSLOCK                 181

#define KEY_F1                       182
#define KEY_F2                       183
#define KEY_F3                       184
#define KEY_F4                       185
#define KEY_F5                       186
#define KEY_F6                       187
#define KEY_F7                       188
#define KEY_F8                       189
#define KEY_F9                       190
#define KEY_F10                      191
#define KEY_F11                      192
#define KEY_F12                      193


#define KEY_SCROLLOCK                195

#define KEY_INSERT                   197
#define KEY_HOME                     198
#define KEY_PAGEUP                   199
#define KEY_END                      201
#define KEY_PAGEDOWN                 202

#define KEY_RIGHT                    203
#define KEY_LEFT                     204
#define KEY_DOWN                     205
#define KEY_UP                       206

/*
	Keypad keys after conversion
*/
#define KEY_NUMLOCK                  207
#define KEY_KP_DIVIDE                208
#define KEY_KP_MULTIPLY              209
#define KEY_KP_MINUS                 210
#define KEY_KP_PLUS                  211
#define KEY_KP_ENTER                 212
#define KEY_KP1                      213
#define KEY_KP2                      214
#define KEY_KP3                      215
#define KEY_KP4                      216
#define KEY_KP5                      217
#define KEY_KP6                      218
#define KEY_KP7                      219
#define KEY_KP8                      220
#define KEY_KP9                      221
#define KEY_KP0                      222
#define KEY_KP_PERIOD                223


#define KEY_LCTRL                    348
#define KEY_LSHIFT                   349
#define KEY_LALT                     350
#define KEY_RCTRL                    352
#define KEY_RSHIFT                   353
#define KEY_RALT                     354

#define KEY_LAST					 355
