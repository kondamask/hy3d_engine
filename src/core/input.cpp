#include "input.h"

namespace HY3D
{

	namespace Input
	{
		global_var input_state state;
	} // namespace Input

	void InputProcessButton(input_button button, bool pressed)
	{
		if (Input::state.isPressed[(u32)button] != pressed)
		{
			Input::state.isPressed[(u32)button] = pressed;

			if (pressed)
			{
				LOG_DEBUG("%s pressed", Input::buttonStr[(u32)button]);
			}
			else
			{
				LOG_DEBUG("%s released", Input::buttonStr[(u32)button]);
			}
		}
		else
		{
			Input::state.wasPressed[(u32)button] = true;
		}
	}

	bool InputIsKeyPressed(input_button button)
	{
		return Input::state.isPressed[(u32)button];
	}
} // namespace HY3D
