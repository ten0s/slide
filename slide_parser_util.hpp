#ifndef __SLIDE_PARSER_UTIL_HPP__
#define __SLIDE_PARSER_UTIL_HPP__

#include <cstdint>
#include <stdexcept>
#include "slide_endian.hpp"

template<typename T>
T read(const uint8_t buf[sizeof(T)], Endian endian) {
    union { uint8_t in[sizeof(T)]; T out; } x;
    switch (endian) {
    case Endian::LE:
        for (size_t i = 0; i < sizeof(T); ++i) {
            x.in[i] = buf[i];
        }
        break;
    case Endian::BE:
        for (size_t i = 0; i < sizeof(T); ++i) {
            x.in[i] = buf[sizeof(T)-1-i];
        }
        break;
    default:
        throw std::runtime_error{"Unknown endian"};
    }
    return x.out;
}

template<typename T>
uint8_t high_order_byte(T val) {
    union { T in; uint8_t out[sizeof(T)]; } x;
    x.in = val;
    return x.out[sizeof(T)-1];
}

template<typename T>
uint8_t low_order_byte(T val) {
    union { T in; uint8_t out[sizeof(T)]; } x;
    x.in = val;
    return x.out[0];
}

#endif // __SLIDE_PARSER_UTIL_HPP__
