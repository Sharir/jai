#ifndef _PREPROCESSOR_H
#define _PREPROCESSOR_H

#include <string>
#include <algorithm>

std::string processFile(std::string);

int count_occurences(std::string, char, size_t, size_t);
std::string prepare(std::string);

#endif
