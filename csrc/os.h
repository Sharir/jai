#ifndef _OS_H
#define _OS_H

// Adapted from https://sourceforge.net/p/predef/wiki/OperatingSystems/

/*

Main:
		OS_LNX - linux
		OS_MAC - mac
		OS_WIN - windows

Additional:
		OS_UNX - any unix compliant os
		OS_WIN32 - 32bit windows
		OS_WIN64 - 64bit windows
		OS_CYG - cygwin

*/

// _WIN32 is usually also defined in 64bit compilers. We want this to take precedence
#ifdef _WIN64
	#define OS_WIN 1
	#define OS_WIN64 1
#elif _WIN32
  #define OS_WIN 1
	#define OS_WIN32 1
#elif __APPLE__ && __MACH__
	#define OS_UNX 1
  #define OS_MAC 1
#elif __linux__
	#define OS_UNX 1
  #define OS_LNX 1
#elif __unix__
  #define OS_UNX 1
#elif __CYGWIN__
	#define OS_WIN 1
	#define OS_CYG 1
// #elif defined(_POSIX_VERSION)
//   #define OS_PSX
#else
	#error "Unknown compiler"
#endif

#endif
