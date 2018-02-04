#include "canvas.h"
#include <cmath>
#include <QPainter>
#include <board.h>

using namespace std;
using namespace board;

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
{
}

Canvas::~Canvas()
{
}

void Canvas::resizeBoard(int size)
{
    assert(5 < size && size < 19);
    _size = size;
}

void Canvas::resizeEvent(QResizeEvent * event)
{
    if (_ratio * width() > height())
    {
        _h = height();
        _w = _h / _ratio;
    }
    else
    {
        _w = width();
        _h = _w * _ratio;
    }
    if (_h < 200 || _w < 200)
        return;
    int mid = _size / 2;
    auto row_block_cnt = (1 + _size) + (_size + 1) / 2;
    _hex_w = (double)_w / row_block_cnt;
    _hex_h = _hex_w * sqrt(3) * 2.0 / 3.0;

    _ct[mid][mid].setX(_w / 2.0);
    _ct[mid][mid].setY(_h / 2.0);

    for (int row = 0; row < _size; ++row)
    {
        for (int col = 0; col < _size; ++col)
        {
            _ct[row][col].setX(_ct[mid][mid].x() + (col - mid) * _hex_w + (row - mid) * 0.5 * _hex_w);
            _ct[row][col].setY(_ct[mid][mid].y() + (row - mid) * _hex_h);
        }
    }
}

void Canvas::paintEvent(QPaintEvent * event)
{
    if (_h < 200 || _w < 200)
        return;
    renderEmptyBoard();

    QPainter painter(this);
    painter.setPen(QPen(Qt::blue, 4));//设置画笔形式
    //painter.drawLine(20, 20, 220, 220);//画直线
    //painter.drawLine(20, 220, 220, 20);
    painter.drawEllipse(_ct[_size / 2][_size / 2],
                        _hex_w / 2,
                        _hex_w / 2);
    //painter.drawRect(20, 20, 200, 200);//画矩形

    for (int row = 0; row < _size; ++row)
    {
        for (int col = 0; col < _size; ++col)
        {
            painter.drawPoint(_ct[row][col]);
            //painter.drawEllipse(_ct[row][col],
            //                    _hex_h / 2,
            //                    _hex_h / 2);
        }
    }

    static const QPointF points[4] = {
        QPointF(10.0, 80.0),
        QPointF(20.0, 10.0),
        QPointF(80.0, 30.0),
        QPointF(90.0, 70.0)
    };

    //painter.drawConvexPolygon(points, 4);
}

void Canvas::renderEmptyBoard()
{
}

void Canvas::renderBorder()
{
}

void Canvas::renderPieces()
{
}

void Canvas::renderInfo()
{
}
