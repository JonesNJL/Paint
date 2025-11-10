#include "Controls.hpp"

Controls::Controls(GLFWwindow* window, int screenHeight)
{
	this->window = window;
	this->screenHeight = screenHeight;
}

void Controls::CreateKeybinds()
{
	keybinds.emplace("left_bracket", Keybind(GLFW_KEY_LEFT_BRACKET, false));
	keybinds.emplace("right_bracket", Keybind(GLFW_KEY_RIGHT_BRACKET, false));
	keybinds.emplace("left_mouse", Keybind(GLFW_MOUSE_BUTTON_LEFT, true));
	keybinds.emplace("w", Keybind(GLFW_KEY_W, false));
	keybinds.emplace("a", Keybind(GLFW_KEY_A, false));
	keybinds.emplace("s", Keybind(GLFW_KEY_S, false));
	keybinds.emplace("d", Keybind(GLFW_KEY_D, false));
	keybinds.emplace("q", Keybind(GLFW_KEY_Q, false));
	keybinds.emplace("e", Keybind(GLFW_KEY_E, false));
}

void Controls::Update()
{
	std::map<std::string, Keybind>::iterator it;
	for (it = keybinds.begin(); it != keybinds.end(); it++)
	{
		Keybind& keybind = it->second;

		int glfwState = 0;
		if (keybind.isMouse) { glfwState = glfwGetMouseButton(window, keybind.glfwKey); }
		else				 { glfwState = glfwGetKey(window, keybind.glfwKey); }
		

		if (glfwState == GLFW_PRESS)
		{
			if (keybind.pressState == 1 || keybind.pressState == 2)
			{
				keybind.pressState = 2;
			}
			else
			{
				keybind.pressState = 1;
			}
		}
		else
		{
			if (keybind.pressState == 3 || keybind.pressState == 0)
			{
				keybind.pressState = 0;
			}
			else
			{
				keybind.pressState = 3;
			}
		}
	}
}

bool Controls::KeyDown(std::string keyID)
{
	if (this->keybinds.at(keyID).pressState == 1 || this->keybinds.at(keyID).pressState == 2)
	{
		return true;
	}

	return false;
}

bool Controls::KeyHit(std::string keyID)
{
	if (this->keybinds.at(keyID).pressState == 1)
	{
		return true;
	}

	return false;
}

bool Controls::KeyRelease(std::string keyID)
{
	if (this->keybinds.at(keyID).pressState == 3)
	{
		return true;
	}

	return false;
}

Int2 Controls::MousePos()
{
	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);

	return Int2(xPos, screenHeight - yPos);
}