#ifndef CONTROLS_H
#define CONTROLS_H

#include <string>
#include <map>
#include <GLFW/glfw3.h>
#include "Util.hpp";

class Controls
{
	private:
		struct Keybind
		{
			int glfwKey;
			bool isMouse;
			int pressState = 0; //0: Up, 1: Hit, 2: Down, 3: Release

			Keybind(int glfwKey, bool isMouse) { this->glfwKey = glfwKey; this->isMouse = isMouse; };
		};
		std::map<std::string, Keybind> keybinds;
		GLFWwindow* window;
		int screenHeight;
	public:
		Controls() = default;
		Controls(GLFWwindow* window, int screenHeight);
		void CreateKeybinds();
		void Update();
		bool KeyDown(std::string keyID);
		bool KeyHit(std::string keyID);
		bool KeyRelease(std::string keyID);
		Int2 MousePos();
};

#endif