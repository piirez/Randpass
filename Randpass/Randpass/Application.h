#pragma once
#include "ImApp/Renderer.h"
#include <string>
#include <random>
#include "Database.h"

class Application
{
private:
	std::string password;
	bool loggedIn;
	int wrongPassCount;
	std::string addedPassText;
	std::string foundPassText;
	std::string deletedPassText;

	Database* passwordList;

	bool justLoggedIn;

	std::string nameSet;
	std::string passwordSet;
	std::string nameGet;
	std::string nameDelete;

	std::string generateRandomPassword(int length);

	void filterText(std::string& value);
public:
	bool useMain;

	Application();
	void Update();
};

