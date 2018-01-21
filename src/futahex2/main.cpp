#include "app.h"
#include <memory>
#include <QtWidgets/QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto desktop = a.desktop();
    auto rect = desktop->screenGeometry();
    int height = rect.height() * 0.5f;

    app w;

    w.setFixedSize(height * 16 / 9, height);
    w.show();

    return a.exec();
}
