#include "slide_cache.hpp"
#include "slide.hpp"

SlideCache::SlideCache() {}

SlideCache::~SlideCache()
{
    for (auto& [_key, slide] : _cache) {
        delete slide;
    }
    _cache = {};
}

const Slide*
SlideCache::find(const std::string& slide_uri) const
{
    auto it = _cache.find(slide_uri);
    if (it != _cache.end()) {
        return it->second;
    } else {
        return nullptr;
    }
}

void
SlideCache::store(const std::string& slide_uri, Slide* slide)
{
    _cache.insert({slide_uri, slide});
}
