#include <string>
#include <ostream>
#include "logger.h"

namespace board
{
template<typename Test, int size>
inline BoardT<Test, size>::BoardT() noexcept
{
    check_boardsize();
}

template<typename Test, int size>
inline void board::BoardT<Test, size>::check_boardsize()
{
    using namespace std;
    using namespace logger;
    if (0 == size % 2)
    {
        ostringstream oss;
        oss << __func__ << "(" << size << ")" << " illegal!" << endl;
        oss << "board size supposed to an odd number greater than five.";

        debug(Level::Fatal) << oss.str();
        throw;
    }
}

}
