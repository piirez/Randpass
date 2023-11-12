#pragma once
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Data.h"
#include "Crypto.h"

class Database
{
private:
	std::map<std::string, DataItem*> data;
public:
	void SaveData(const char* inputFile, std::string Password);
	bool LoadData(const char* inputFile, std::string Password);

	std::string GetString(std::string Key);
	void SetString(std::string Key, std::string Value);

	bool DeleteData(std::string Key);

	std::map<std::string, DataItem*> GetData();
};

