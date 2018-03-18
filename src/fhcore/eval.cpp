#include <memory>
#include "board.h"
#include "eval.h"
using namespace std;
using namespace board;
namespace eval
{

Eval::Eval(board::IBoard * pBoard)
    : _pBoard(pBoard)
{
}

Eval::operator long()
{
    return 0;
}

}
