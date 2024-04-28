#include "Console.h"

#include <iostream>
#include <typeinfo>

void Console::OutputLine(const std::string& msg) {
	// Write to iostream (but we could do other stuff here too, like write to log files)
	std::cout << msg << std::endl;
}

void Console::Info(const std::string& msg) {
	OutputLine(msg);
}

void Console::Warning(const std::string& msg) {
	OutputLine("[WARNING] + " + msg);
}

void Console::Error(const std::string& msg) {
	OutputLine("[ERROR] + " + msg);
}
void Console::Error(const std::exception& e) {
	Error(typeid(e).name() + std::string(" : ") + e.what());
}