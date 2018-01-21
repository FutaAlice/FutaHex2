#include "app.h"
#include <QPainterPath>
#include <QPainter>

app::app(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.centralWidget->setLayout(ui.mainLayout);
}

void app::paintEvent(QPaintEvent * event)
{
}
