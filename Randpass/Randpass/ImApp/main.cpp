#include "Gui.h"

int main()
{
	ShowWindow(GetConsoleWindow(), SW_SHOW);

	std::cout << "   ___               __                \n" <<
		"  / _ \\___ ____  ___/ /__  ___ ____ ___\n" <<
		" / , _/ _ `/ _ \\/ _  / _ \\/ _ `(_-<(_-<\n" <<
		"/_/|_|\\_,_/_//_/\\_,_/ .__/\\_,_/___/___/\n" <<
		"                   /_/                 \n";

	Application app = Application();

	Gui* gui = new Gui(1920 / 2, 1080 / 2, "Randpass", NULL, NULL);
	GLFWwindow* window = gui->GetWindow();
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	while (!glfwWindowShouldClose(window))
	{
		// render
		// ------
		gui->StartDraw(app.useMain);

		app.Update();

		gui->EndDraw(app.useMain);
	};

	gui->CleanUp();
}