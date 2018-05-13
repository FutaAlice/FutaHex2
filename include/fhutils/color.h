#pragma once
#include <cassert>
#include <iostream>

namespace fhutils {
namespace color {

using color_t = char;

enum class Color :color_t {
    Empty = -1, Red = 0, Blue = 1,
};

// Get opposite color.
Color operator!(const Color color);

// Convert to color_t.
constexpr color_t operator*(const Color color) {
    return static_cast<color_t>(color);
}

std::ostream& operator<< (std::ostream& stream, Color color);

} // namespace color
} // namespace fhutils
