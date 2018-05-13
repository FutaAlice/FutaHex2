#include "entry.h"
#include <QtWidgets/QApplication>
#include "app.h"
#include "entry.h"

using fhutils::iengine::IEngine;

namespace fhgui {

Entry & Entry::instance(IEngine *engine) {
    static Entry entry(engine);
    return entry;
}

int Entry::exec(int argc, char *argv[]) {
    QApplication a(argc, argv);
    app hex;
    hex.show();
    hex.pEngine = engine_;
    return a.exec();
}

Entry::Entry(IEngine *engine) : engine_(engine) {
}

} // namespace fhgui
