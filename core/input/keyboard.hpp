#ifndef _ISM_KEYBOARD_HPP_
#define _ISM_KEYBOARD_HPP_

#include <core/typedefs.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// key codes
	ENUM_INT(KeyCode)
	{
		KeyCode_Space			= 32,	// space
		KeyCode_Apostrophe		= 39,	// apostrophe
		KeyCode_Comma			= 44,	// comma
		KeyCode_Minus			= 45,	// minus
		KeyCode_Period			= 46,	// period
		KeyCode_Slash			= 47,	// slash
		KeyCode_Num0			= 48,	// 0
		KeyCode_Num1			= 49,	// 1
		KeyCode_Num2			= 50,	// 2
		KeyCode_Num3			= 51,	// 3
		KeyCode_Num4			= 52,	// 4
		KeyCode_Num5			= 53,	// 5
		KeyCode_Num6			= 54,	// 6
		KeyCode_Num7			= 55,	// 7
		KeyCode_Num8			= 56,	// 8
		KeyCode_Num9			= 57,	// 9
		KeyCode_Semicolon		= 59,	// semicolon
		KeyCode_Equal			= 61,	// equal
		KeyCode_A				= 65,	// a
		KeyCode_B				= 66,	// b
		KeyCode_C				= 67,	// c
		KeyCode_D				= 68,	// d
		KeyCode_E				= 69,	// e
		KeyCode_F				= 70,	// f
		KeyCode_G				= 71,	// g
		KeyCode_H				= 72,	// h
		KeyCode_I				= 73,	// i
		KeyCode_J				= 74,	// j
		KeyCode_K				= 75,	// k
		KeyCode_L				= 76,	// l
		KeyCode_M				= 77,	// m
		KeyCode_N				= 78,	// n
		KeyCode_O				= 79,	// o
		KeyCode_P				= 80,	// p
		KeyCode_Q				= 81,	// q
		KeyCode_R				= 82,	// r
		KeyCode_S				= 83,	// s
		KeyCode_T				= 84,	// t
		KeyCode_U				= 85,	// u
		KeyCode_V				= 86,	// v
		KeyCode_W				= 87,	// w
		KeyCode_X				= 88,	// x
		KeyCode_Y				= 89,	// y
		KeyCode_Z				= 90,	// z
		KeyCode_LeftBracket		= 91,	// left bracket
		KeyCode_Backslash		= 92,	// backslash
		KeyCode_RightBracket	= 93,	// right bracket
		KeyCode_GraveAccent		= 96,	// grave accent
		KeyCode_World1			= 161,	// world 1
		KeyCode_World2			= 162,	// world 2
		KeyCode_Escape			= 256,	// escape
		KeyCode_Enter			= 257,	// enter
		KeyCode_Tab				= 258,	// tab
		KeyCode_Backspace		= 259,	// backspace
		KeyCode_Insert			= 260,	// insert
		KeyCode_Del				= 261,	// del
		KeyCode_Right			= 262,	// right
		KeyCode_Left			= 263,	// left
		KeyCode_Down			= 264,	// down
		KeyCode_Up				= 265,	// up
		KeyCode_PageUp			= 266,	// page up
		KeyCode_PageDown		= 267,	// page down
		KeyCode_Home			= 268,	// home
		KeyCode_End				= 269,	// end
		KeyCode_CapsLock		= 280,	// caps lock
		KeyCode_ScrollLock		= 281,	// scroll lock
		KeyCode_NumLock			= 282,	// num lock
		KeyCode_PrintScreen		= 283,	// print screen
		KeyCode_Pause			= 284,	// pause
		KeyCode_Fn1				= 290,	// f1
		KeyCode_Fn2				= 291,	// f2
		KeyCode_Fn3				= 292,	// f3
		KeyCode_Fn4				= 293,	// f4
		KeyCode_Fn5				= 294,	// f5
		KeyCode_Fn6				= 295,	// f6
		KeyCode_Fn7				= 296,	// f7
		KeyCode_Fn8				= 297,	// f8
		KeyCode_Fn9				= 298,	// f9
		KeyCode_Fn10			= 299,	// f10
		KeyCode_Fn11			= 300,	// f11
		KeyCode_Fn12			= 301,	// f12
		KeyCode_Fn13			= 302,	// f13
		KeyCode_Fn14			= 303,	// f14
		KeyCode_Fn15			= 304,	// f15
		KeyCode_Fn16			= 305,	// f16
		KeyCode_Fn17			= 306,	// f17
		KeyCode_Fn18			= 307,	// f18
		KeyCode_Fn19			= 308,	// f19
		KeyCode_Fn20			= 309,	// f20
		KeyCode_Fn21			= 310,	// f21
		KeyCode_Fn22			= 311,	// f22
		KeyCode_Fn23			= 312,	// f23
		KeyCode_Fn24			= 313,	// f24
		KeyCode_Fn25			= 314,	// f25
		KeyCode_Kp0				= 320,	// keypad 0
		KeyCode_Kp1				= 321,	// keypad 1
		KeyCode_Kp2				= 322,	// keypad 2
		KeyCode_Kp3				= 323,	// keypad 3
		KeyCode_Kp4				= 324,	// keypad 4
		KeyCode_Kp5				= 325,	// keypad 5
		KeyCode_Kp6				= 326,	// keypad 6
		KeyCode_Kp7				= 327,	// keypad 7
		KeyCode_Kp8				= 328,	// keypad 8
		KeyCode_Kp9				= 329,	// keypad 9
		KeyCode_KpDecimal		= 330,	// keypad decimal
		KeyCode_KpDivide		= 331,	// keypad divide
		KeyCode_KpMultiply		= 332,	// keypad multiply
		KeyCode_KpSubtract		= 333,	// keypad subtract
		KeyCode_KpAdd			= 334,	// keypad add
		KeyCode_KpEnter			= 335,	// keypad enter
		KeyCode_KpEqual			= 336,	// keypad equal
		KeyCode_LeftShift		= 340,	// left shift
		KeyCode_LeftCtrl		= 341,	// left control
		KeyCode_LeftAlt			= 342,	// left alt
		KeyCode_LeftSuper		= 343,	// left super
		KeyCode_RightShift		= 344,	// right shift
		KeyCode_RightCtrl		= 345,	// right control
		KeyCode_RightAlt		= 346,	// right alt
		KeyCode_RightSuper		= 347,	// right super
		KeyCode_Menu			= 348,	// menu

		KeyCode_MAX
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// key modifiers
	ENUM_INT(KeyMods)
	{
		KeyMods_None	= 0 << 0,	// nullptr
		KeyMods_Shift	= 0 << 1,	// shift
		KeyMods_Ctrl	= 1 << 1,	// ctrl
		KeyMods_Alt		= 1 << 2,	// alt
		KeyMods_Super	= 1 << 3,	// super
		KeyMods_Caps	= 1 << 4,	// caps
		KeyMods_NumLock	= 1 << 5,	// numlock
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_KEYBOARD_HPP_
