#include "Logger.h"

Logger::Logger() {}

bool Logger::enabled = false;
int Logger::logCount = 0;

void Logger::Log(std::string out)
{
	if (enabled)
	{
		std::cout << "[" << logCount << "]: " << out << std::endl;
		logCount += 1;
	}
}