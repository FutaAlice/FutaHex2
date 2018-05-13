#include <memory>
#include <fhutils/board.h>
#include "eval.h"

using namespace fhutils::board;

namespace fhcore {
namespace eval {

Eval::Eval(fhutils::board::IBoard * pBoard)
    : _pBoard(pBoard) {
}

Eval::operator long() {
    return 0;
}

} // namespace eval
} // namespace fhcore
