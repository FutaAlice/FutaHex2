# Samples of fhcore usage

## Example 1

compute and determine next move using fhcore.

```cpp
#include <iostream>
#include <cstdio>
#include <chrono>
#include <fhcore/board.h>
#include <fhcore/mcts.h>
using namespace std;
using namespace board;
using namespace engine;

int main() {
    // Initialize an empty 11*11 hex gameboard.
    auto *ptrBoard = IBoard::create(11);
    auto &refBoard = *ptrBoard;

    // Put a RED piece at locate(row:5, col: 5).
    refBoard(5, 5) = Color::Red;
    cout << refBoard.debug_state_str() << refBoard.debug_bit_str() << endl;

    // Create a MCTS engine with time limit 20 seconds (default 60 seconds).
    IEngine *ptrEngine = new MCTSEngine(chrono::seconds(20));

    // Configure engine.
    Color nextColor = refBoard.color();
    ptrEngine->configure(EngineCfg{ &refBoard, refBoard.color() });

    // Engine determine next move.
    pos_t nextMove;
    ptrEngine->compute_sync(nextMove);

    // Set piece on gameboard.
    refBoard(nextMove.index) = nextColor;
    
    cout << refBoard.debug_state_str() << refBoard.debug_bit_str() << endl;
    printf("MCTS engine make move at (row:%d, col:%d).\n", nextMove.row, nextMove.col);

}
```