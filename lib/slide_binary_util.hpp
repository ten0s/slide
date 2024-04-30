/**
 *   AutoCAD slide library
 *
 *   Copyright (C) 2023-2024 Dmitry Klionsky aka ten0s <dm.klionsky@gmail.com>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* SPDX-License-Identifier: GPL-3.0-or-later */

#ifndef __SLIDE_BINARY_UTIL_HPP__
#define __SLIDE_BINARY_UTIL_HPP__

#include <cstdint>
#include <stdexcept>
#include <ostream>
#include "slide_endian.hpp"

namespace libslide {

template<typename T>
T read(const uint8_t buf[sizeof(T)], Endian endian)
{
    union { uint8_t in[sizeof(T)]; T out; } x;
    switch (endian) {
    case Endian::little:
        for (size_t i = 0; i < sizeof(T); ++i) {
            x.in[i] = buf[i];
        }
        break;
    case Endian::big:
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
void write(uint8_t buf[sizeof(T)], T val, Endian endian)
{
    union { T in; uint8_t out[sizeof(T)]; } x;
    x.in = val;
    switch (endian) {
    case Endian::little:
        for (size_t i = 0; i < sizeof(T); ++i) {
            buf[i] = x.out[i];
        }
        break;
    case Endian::big:
        for (size_t i = 0; i < sizeof(T); ++i) {
            buf[sizeof(T)-1-i] = x.out[i];
        }
        break;
    default:
        throw std::runtime_error{"Unknown endian"};
    }
}

template<typename T>
std::ostream&
write(std::ostream& os, T val, Endian endian)
{
    uint8_t buf[sizeof(T)];
    write(buf, val, endian);
    os.write((char*)buf, sizeof(buf));
    return os;
}

template<typename T>
T make(std::initializer_list<uint8_t> digits)
{
    T out {};
    for (auto digit : digits) {
        out = (out << 8) + digit;
    }
    return out;
}

template<typename T>
uint8_t high_order_byte(T val)
{
    union { T in; uint8_t out[sizeof(T)]; } x;
    x.in = val;
    return x.out[sizeof(T)-1];
}

template<typename T>
uint8_t low_order_byte(T val)
{
    union { T in; uint8_t out[sizeof(T)]; } x;
    x.in = val;
    return x.out[0];
}

} // namespace libslide

#endif // __SLIDE_BINARY_UTIL_HPP__
