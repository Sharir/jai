#ifndef _COMPILER_H
#define _COMPILER_H

#include <string>
#include "token.h"

#define COMPILER_DEBUG 1

enum LogLevel {
#if COMPILER_DEBUG
	DEBUG,
#endif
	INFO,
	WARN,
	ERROR,
};

void compileFile(std::string);

void logCompiler(LogLevel, const char*, Token* = NULL);

#endif
