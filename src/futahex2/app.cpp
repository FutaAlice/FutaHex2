#include "app.h"
#include <QPainterPath>
#include <QPainter>

app::app(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.centralWidget->setLayout(ui.mainLayout);
    setFixedSize(w * 16 / 9, w);
}

void app::paintEvent(QPaintEvent * event)
{
}
