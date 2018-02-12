#include "app.h"
#include <cassert>
#include <QtWidgets/QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto desktop = a.desktop();
    auto rect = desktop->screenGeometry();
    assert(app::w < rect.height());

    app w;
    w.show();

    return a.exec();
}
