#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

#include <string>
#include <vector>

namespace StringUtils {

std::string uppercase(const std::string &string);
std::vector<std::string> splitLines(const std::string &string);
std::string trimSpaces(const std::string &string);

}
#endif