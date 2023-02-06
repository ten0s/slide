#ifndef __SLIDE_UTIL_HPP__
#define __SLIDE_UTIL_HPP__

#include <string>

std::string basename(const std::string& filename);
std::string get_ext(const std::string& filename);
std::string strip_ext(const std::string& filename);
std::string to_upper(const std::string& str);

#endif // __SLIDE_UTIL_HPP__
