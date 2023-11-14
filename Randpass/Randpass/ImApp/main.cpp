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

	double lastFrameTime = glfwGetTime();
	const double targetFPS = 20.0;
	const double targetFrameTime = 1.0 / targetFPS;

	while (!glfwWindowShouldClose(window))
	{
		double currentFrameTime = glfwGetTime();
		double deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		// render
		gui->StartDraw(app.useMain);
		app.Update();
		gui->EndDraw(app.useMain);

		double sleepTime = targetFrameTime - deltaTime;
		// If there is still time left in the frame, sleep
		if (sleepTime > 0.0) 
		{
			glfwWaitEventsTimeout(sleepTime);
		}
	};

	gui->CleanUp();
}