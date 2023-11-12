#pragma once
#include <string>

enum DataType
{
	String
};

struct DataItem
{
	DataType Type;
	std::string Value;
};