#ifndef __SLIDE_LOADER_HPP__
#define __SLIDE_LOADER_HPP__

#include <string>

class Slide;

const Slide* slide_from_uri(const std::string& slide_uri);

#endif // __SLIDE_LOADER_HPP__
