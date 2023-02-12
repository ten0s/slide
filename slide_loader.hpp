#ifndef __SLIDE_LOADER_HPP__
#define __SLIDE_LOADER_HPP__

#include <string>

namespace libslide {

class Slide;

const Slide* slide_from_uri(const std::string& slide_uri);

} // namespace libslide

#endif // __SLIDE_LOADER_HPP__
