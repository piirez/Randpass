#pragma once
#include "../Application.h"

class Gui
{
private:
	GLFWwindow* window;
	Renderer renderer;
public:
	Gui(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share);

	GLFWwindow* GetWindow();

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void StartDraw(bool useMain);
	void EndDraw(bool useMain);
	void CleanUp();
};