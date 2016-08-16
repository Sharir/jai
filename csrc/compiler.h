#ifndef _COMPILER_H
#define _COMPILER_H

#include <string>
#include "token.h"

#define COMPILER_DEBUG

enum LogLevel {
	DEBUG,
	INFO,
	WARN,
	ERROR,
};

void compileFile(std::string);

void logCompiler(LogLevel, const char*, Token* = NULL);

#endif
