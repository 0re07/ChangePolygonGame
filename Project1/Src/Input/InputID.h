#pragma once
#include<type_traits>

enum class InputID
{
	Up,
	Down,
	Left,
	Right,
	FrontSide,
	BackSide,
	LeftSide,
	RightSide,
	Btn1,
	Btn2,
	Btn3,
	Max
};

//”ÍˆÍfor•¶ŠÖ˜A
static InputID begin(InputID)
{
	return InputID::Up;
}

static InputID end(InputID)
{
	return InputID::Max;
}

static InputID operator++(InputID& id)
{
	id = static_cast<InputID>(std::underlying_type<InputID>::type(id) + 1);
	return id;
}

static InputID operator*(InputID& id)
{
	return id;
}
