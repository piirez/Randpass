#pragma once
#include "ImApp/Renderer.h"
#include <string>
#include <random>
#include "Database.h"

class Application
{
private:
	Database* passwordList;

	bool loggedIn;
	bool justLoggedIn;
	int wrongPassCount;

	std::string password;

	std::string addedPassType;
	std::string addedPassText;
	std::string foundPassText;
	std::string deletedPassText;

	std::string accountSet;
	std::string passwordSet;
	std::string accountGet;
	std::string accountDelete;

	std::string generateRandomPassword(int length);

	void filterText(std::string& value);
public:
	bool useMain;

	Application();
	void Update();
};

