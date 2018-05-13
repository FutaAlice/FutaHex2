#include <chrono>
#include <fhgui/entry.h>
#include <fhcore/mcts.h>

using std::chrono::seconds;
using fhgui::Entry;
using fhcore::mcts::MCTSEngine;

int main(int argc, char *argv[]) {
    auto pEngine = new MCTSEngine(seconds(20));
    Entry &entry = Entry::instance(pEngine);
    return entry.exec(argc, argv);
}