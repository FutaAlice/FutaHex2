#pragma once

namespace color
{
using color_t = unsigned char;

enum class Color :color_t
{
    Red = 0,
    Blue,
};

constexpr Color operator!(const Color color)
{
    return static_cast<Color>(!static_cast<color_t>(color));
}

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
