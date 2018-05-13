#include <map>
#include <iomanip>
#include "color.h"

using namespace std;

namespace fhutils {
namespace color {

Color operator!(const Color color) {
    assert(color != Color::Empty);
    return static_cast<Color>(!static_cast<color_t>(color));
}

ostream & operator<<(ostream & stream, Color color) {
    // stream << std::setfill(' ') << std::setw(5);
    stream << setfill(' ') << setw(1);
    static const map<Color, const char *> str_map =
    {
        { Color::Red, "r" },
        { Color::Blue, "b" },
        { Color::Empty, "_" },
    };
    stream << str_map.find(color)->second;

    return stream;
}

} // namespace color
} // namespace fhutils
