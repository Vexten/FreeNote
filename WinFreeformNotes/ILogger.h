#pragma once
#include <string>
#include <format>

class ILogger
{
public:
	virtual bool write(std::string str) = 0;
};

