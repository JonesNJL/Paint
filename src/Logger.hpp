#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>

class Logger
{
	public:
		Logger();
		static bool enabled;
		static int logCount;
		static void Log(std::string out);
};

#endif