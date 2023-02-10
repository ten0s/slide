#ifndef __SLIDE_CACHE_HPP__
#define __SLIDE_CACHE_HPP__

#include <string>
#include <unordered_map>

class Slide;

class SlideCache {
public:
    explicit SlideCache();
    ~SlideCache();

    SlideCache(const SlideCache&) = delete;
    SlideCache(SlideCache&&) = delete;
    SlideCache& operator=(const SlideCache&) = delete;
    SlideCache& operator=(SlideCache&&) = delete;

    const Slide* find(const std::string& slide_uri) const;
    void store(const std::string& slide_uri, Slide* slide);

private:
    std::unordered_map<std::string, Slide*> _cache;
};

#endif // __SLIDE_CACHE_HPP__
