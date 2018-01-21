#include "canvas.h"
#include <QPainter>

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
{
}

Canvas::~Canvas()
{
}

void Canvas::paintEvent(QPaintEvent * event)
{
    static const QPointF points[4] = {
        QPointF(10.0, 80.0),
        QPointF(20.0, 10.0),
        QPointF(80.0, 30.0),
        QPointF(90.0, 70.0)
    };

    QPainter painter(this);
    painter.drawConvexPolygon(points, 4);
}
