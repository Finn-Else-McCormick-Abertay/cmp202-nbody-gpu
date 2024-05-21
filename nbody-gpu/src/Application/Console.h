#pragma once

#include <string>

class Console
{
public:
	void Info(const std::string&);
	void Warning(const std::string&);
	void Error(const std::string&);
	void Error(const std::exception&);

private:
	void OutputLine(const std::string&);
};