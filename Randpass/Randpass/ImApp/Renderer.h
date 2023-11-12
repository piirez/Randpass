#pragma once
#include <Windows.h>
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_internal.h>
#include <GLFW/glfw3.h>
//#define GLFW_EXPOSE_NATIVE_WIN32
//#include <GLFW/glfw3native.h>

class Renderer
{
public:
	void StartDraw(bool useMain);
	void EndDraw(bool useMain);
	void CleanUp();
};

