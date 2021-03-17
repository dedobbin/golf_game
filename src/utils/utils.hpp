#pragma once
#include <string>
#include <vector>

void strip_ext(char *fname);

bool ends_with(char* haystack, char* needle);

std::vector<std::string> explode(std::string const & s, char delim);