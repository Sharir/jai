#ifndef _PREPROCESSOR_H
#define _PREPROCESSOR_H

#include <string>
#include <algorithm>

int count_occurences(std::string, char, size_t, size_t);
std::string prepare(std::string);
std::string processFile(std::string);

#endif
