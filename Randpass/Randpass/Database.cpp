#include "Database.h"

void Database::SaveData(const char* inputFile, std::string Password)
{
	std::ofstream ss;
	std::stringstream stringss;
	stringss << "#\n";

	ss.open(inputFile);

	std::map<std::string, DataItem*>::iterator it = data.begin();

	//Read one line at a time
	while (it != data.end()) 
	{
		std::string Key = it->first;
		DataItem* dataItem = data[Key];

		if (dataItem->Type == DataType::String)
		{
			stringss << "string" << ' ' << Key << ' ' << dataItem->Value << '\n';
		}

		++it;
	}
	std::string encryptedData = Crypto::AESEncrypt(stringss.str(), Password);
	encryptedData = Crypto::Base64Encode(encryptedData);
	ss << encryptedData;
	ss.close();

	//std::cout << "PASSWORD LIST SAVED" << '\n';
}

bool Database::LoadData(const char* inputFile, std::string Password)
{
	std::stringstream ss;
	std::ifstream in_file(inputFile);
	std::string line = "";
	std::string prefix = "";

	std::string key = "";
	std::string value = "";

	//File open error check
	if (!in_file.is_open())
	{
		//std::cout << "PASSWORD LIST NOT FOUND, GENERATING NEW ONE" << '\n';
		//SaveData(inputFile, Password);
		return true;
	}

	std::string rawFile = "";
	std::getline(in_file, rawFile);
	std::string decryptedFile = Crypto::Base64Decode(rawFile);
	decryptedFile = Crypto::AESDecrypt(decryptedFile, Password);

	if (decryptedFile == "Fail")
	{
		//std::cout << "Wrong password" << '\n';
		return false;
	}

	std::istringstream f(decryptedFile);

	//Read one line at a time
	while (std::getline(f, line))
	{
		//Get the prefix of the line
		ss.clear();
		ss.str(line);
		ss >> prefix;
		if (prefix == "string")
		{
			ss >> key >> value;
			SetString(key, value);
		}
	}

	//std::cout << "LOADED PASSWORD LIST" << '\n';

	return true;
}

std::string Database::GetString(std::string Key)
{
	if (data.find(Key) == data.end())
	{
		return "Not found";
	}
	else
	{
		return data[Key]->Value;
	}
}

void Database::SetString(std::string Key, std::string Value)
{
	if (data.find(Key) == data.end())
	{
		data[Key] = new DataItem();
		data[Key]->Type = DataType::String;
		data[Key]->Value = Value;
	}
	else
	{
		data[Key]->Type = DataType::String;
		data[Key]->Value = Value;
	}
}

bool Database::DeleteData(std::string Key)
{
	if (data.find(Key) == data.end()) 
		return false;

	data.erase(Key);

	return true;
}

std::map<std::string, DataItem*> Database::GetData()
{
	return data;
}
