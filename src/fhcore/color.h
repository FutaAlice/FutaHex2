#pragma once
#include <cassert>

namespace color
{
using color_t = char;

enum class Color :color_t
{
    Empty = -1,
    Red = 0,
    Blue = 1,
};

// Get opposite color.
#if defined(DEBUG) || defined(_DEBUG)
Color
#else if
constexpr Color
#endif
operator!(const Color color)
{
#if defined(DEBUG) || defined(_DEBUG)
    assert(color != Color::Empty);
#endif
    return static_cast<Color>(!static_cast<color_t>(color));
}

// Convert to color_t.
constexpr color_t operator&(const Color color)
{
    return static_cast<color_t>(color);
}

std::ostream& operator<< (std::ostream& stream, Color color)
{
    if (Color::Red == color)
        stream << "Red";
    else // Color::Blue
        stream << "Blue";

    return stream;
}

}
