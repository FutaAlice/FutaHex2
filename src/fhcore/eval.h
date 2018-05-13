#pragma once

namespace fhutils {
namespace board {
class IBoard;
}
}

namespace fhcore {
namespace eval {

class Eval {
public:
    Eval(fhutils::board::IBoard *pBoard);
    operator long();
private:
    fhutils::board::IBoard *_pBoard{ nullptr };
};

} // namespace eval
} // namespace fhcore
