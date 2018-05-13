#pragma once
#include <functional>
#include <fhutils/iengine.h>

namespace fhgui {

class Entry {
public:
    static Entry & instance(fhutils::iengine::IEngine *engine);
    int exec(int argc, char *argv[]);
private:
    Entry(fhutils::iengine::IEngine *engine);
    fhutils::iengine::IEngine *engine_;
};

} // namespace fhgui
