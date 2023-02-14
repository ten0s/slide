#include <iostream>
#include <string>
#include "../lib/slide.hpp"
#include "../lib/slide_library.hpp"
#include "../lib/slide_util.hpp"

using namespace libslide;

void usage(const std::string& prog)
{
    std::cerr << "Usage: " << prog << " <SLIDE.sld | SLIDELIB.slb [NAME]>\n";
}

int main(int argc, char *argv[])
{
   if (argc == 2) {
       std::string filename = argv[1];
       std::string ext = get_ext(filename);
       if (ext == ".sld") {
           Slide slide = Slide::from_file(filename);
           std::cout << slide;
           return 0;
       } else if (ext == ".slb") {
           SlideLibrary library = SlideLibrary::from_file(filename);
           std::cout << library;
           return 0;
       }
   }

   if (argc == 3) {
       std::string filename = argv[1];
       std::string ext = get_ext(filename);
       if (ext == ".slb") {
           std::string name = argv[2];
           SlideLibrary library = SlideLibrary::from_file(filename);
           const Slide* slide = library.find(name);
           if (!slide) {
               try {
                   size_t idx = std::stol(name);
                   slide = library.find(idx);
               } catch (...) { }
           }

           if (slide) {
               std::cout << *slide;
               return 0;
           } else {
               std::cerr << "Slide '" + name + "' not found";
           }
       }
   }

   usage(basename(argv[0]));
   return 1;
}
