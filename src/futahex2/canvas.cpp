#include "canvas.h"
#include <cmath>
#include <map>
#include <QPainter>
#include <board.h>

using namespace std;
using namespace board;

//static const map<Color, QColor> cm =
//{
//    { Color::Red, _cr },
//    { Color::Blue, _cb },
//    { Color::Empty, _bk },
//};
//QColor c = cm.find(color)->second;

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
{
}

Canvas::~Canvas()
{
}

void Canvas::resizeBoard(int size)
{
    assert(5 < size && size < 19 && 1 == size % 2);
    this->resizeEvent(nullptr);
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
    int row_block_cnt = (1 + _size) + (_size + 1) / 2;
    _hex_w = (_w * 1.0f) / row_block_cnt;
    _hex_h = _hex_w * sqrt(3) * 2.0 / 3.0;

    _ct[mid][mid].setX(_w / 2.0);
    _ct[mid][mid].setY(_h / 2.0);

    for (int row = 0; row < _size; ++row)
    {
        for (int col = 0; col < _size; ++col)
        {
            _ct[row][col].setX(_ct[mid][mid].x() + (col - mid) * _hex_w + (row - mid) * 0.5 * _hex_w);
            _ct[row][col].setY(_ct[mid][mid].y() + (row - mid) * (_hex_w * sqrt(3) / 2));
        }
    }
}

void Canvas::paintEvent(QPaintEvent * event)
{
    if (_h < 200 || _w < 200 || _size == 0)
        return;
    renderBorder();
    renderEmptyBoard();
    renderPieces();
    renderInfo();

    for (int row = 0; row < _size; ++row)
    {
        for (int col = 0; col < _size; ++col)
        {
        }
    }
}

void Canvas::renderEmptyBoard()
{
    QPainter painter(this);

    auto drawHex = [&](auto pt, auto w, auto h, auto ratio) {
        painter.setPen(QPen(Qt::black, 1));
        painter.setBrush(QBrush(_bk, Qt::SolidPattern));
        const QPointF points[6] = {
            QPointF(pt.x(), pt.y() - h / 2),
            QPointF(pt.x() + w / 2, pt.y() - h / 4),
            QPointF(pt.x() + w / 2, pt.y() + h / 4),
            QPointF(pt.x(), pt.y() + h / 2),
            QPointF(pt.x() - w / 2, pt.y() + h / 4),
            QPointF(pt.x() - w / 2, pt.y() - h / 4),
        };
        painter.drawConvexPolygon(points, 6);
    };

    for (int row = 0; row < _size; ++row)
    {
        for (int col = 0; col < _size; ++col)
        {
            drawHex(_ct[row][col], _hex_w, _hex_h, 1);
        }
    }
}

void Canvas::renderBorder()
{
}

void Canvas::renderPieces()
{
    QPainter painter(this);

    auto drawEllipse = [&](auto pt, auto color, auto radius, auto ratio) {
        assert(Color::Empty != color);
        QColor c = (Color::Red == color) ? _cr : _cb;
        painter.setPen(QPen(Qt::black, 1));
        painter.setBrush(QBrush(c, Qt::SolidPattern));
        painter.drawEllipse(pt, radius * ratio, radius * ratio);
    };

    for (int row = 0; row < _size; ++row)
    {
        for (int col = 0; col < _size; ++col)
        {
            drawEllipse(_ct[row][col], Color::Red, _hex_w / 2, 0.85f);
        }
    }
}

void Canvas::renderInfo()
{
}
