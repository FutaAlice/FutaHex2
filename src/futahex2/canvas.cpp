#include "canvas.h"
#include <cmath>
#include <map>
#include <QPainter>
#include <board.h>

using namespace std;
using namespace board;

template<typename T>
static void get_hex_vertex(T points[], T pt, double w, double h)
{
    points[0] = T(pt.x(), pt.y() - h / 2);
    points[1] = T(pt.x() + w / 2, pt.y() - h / 4);
    points[2] = T(pt.x() + w / 2, pt.y() + h / 4);
    points[3] = T(pt.x(), pt.y() + h / 2);
    points[4] = T(pt.x() - w / 2, pt.y() + h / 4);
    points[5] = T(pt.x() - w / 2, pt.y() - h / 4);
}

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

void Canvas::resize(IBoard *pb)
{
    if (!pb)
        return;

    delete _pBoard;
    _pBoard = pb->copy();

    this->resizeEvent(nullptr);
}

void Canvas::resizeEvent(QResizeEvent * event)
{
    if (!_pBoard)
        return;

    auto size = _pBoard->boardsize();
    assert(5 < size && size < 19);

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
    int mid = size / 2;
    int row_block_cnt = (1 + size) + (size + 1) / 2;
    _hex_w = (_w * 1.0f) / row_block_cnt;
    _hex_h = _hex_w * sqrt(3) * 2.0 / 3.0;

    _ct[mid][mid].setX(_w / 2.0);
    _ct[mid][mid].setY(_h / 2.0);

    for (int row = 0; row < size; ++row)
    {
        for (int col = 0; col < size; ++col)
        {
            _ct[row][col].setX(_ct[mid][mid].x() + (col - mid) * _hex_w + (row - mid) * 0.5 * _hex_w);
            _ct[row][col].setY(_ct[mid][mid].y() + (row - mid) * (_hex_w * sqrt(3) / 2));
        }
    }
}

void Canvas::paintEvent(QPaintEvent * event)
{
    if (_h < 200 || _w < 200 || !_pBoard)
        return;
    renderBorder();
    renderEmptyBoard();
    renderPieces();
    renderInfo();

}

void Canvas::renderEmptyBoard()
{
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 1));
    painter.setBrush(QBrush(_bk, Qt::SolidPattern));
    auto drawHex = [&](auto pt, auto w, auto h, auto ratio) {
        QPointF points[6];
        get_hex_vertex(points, pt, w, h);
        painter.drawConvexPolygon(points, 6);
    };

    auto size = _pBoard->boardsize();
    for (int row = 0; row < size; ++row)
    {
        for (int col = 0; col < size; ++col)
        {
            drawHex(_ct[row][col], _hex_w, _hex_h, 1);
        }
    }
}

void Canvas::renderBorder()
{
    auto size = _pBoard->boardsize();

    QPainter painter(this);
    QPointF points[6];
    painter.setPen(QPen(Qt::transparent, 0));
    int mid = size / 2;
    QPainterPath *path = nullptr;
    const double ratio = 1.25;
    auto w = _hex_w * ratio;
    auto h = _hex_h * ratio;
    auto lineTo = [&](int i1, int i2, int i3, bool half) {
        path->moveTo(_ct[mid][mid]);
        path->lineTo(points[i1]);
        path->lineTo(points[i2]);
        if (!half)
            path->lineTo(points[i3]);
        else
            path->lineTo((points[i2].x() + points[i3].x()) / 2,
                         (points[i2].y() + points[i3].y()) / 2);
    };

    path = new QPainterPath;
    path->setFillRule(Qt::WindingFill);
    for (int col = 0; col < size; ++col)
    {
        get_hex_vertex(points, _ct[0][col], w, h);
        lineTo(5, 0, 1, col == size - 1);
        get_hex_vertex(points, _ct[size - 1][col], w, h);
        lineTo(2, 3, 4, col == 0);
    }
    painter.setBrush(QBrush(_cr, Qt::SolidPattern));
    painter.drawPath(*path);
    delete path;
    path = new QPainterPath;
    path->setFillRule(Qt::WindingFill);
    for (int row = 0; row < size; ++row)
    {
        get_hex_vertex(points, _ct[row][0], w, h);
        lineTo(5, 4, 3, row == size - 1);
        get_hex_vertex(points, _ct[row][size - 1], w, h);
        lineTo(2, 1, 0, row == 0);
    }
    painter.setBrush(QBrush(_cb, Qt::SolidPattern));
    painter.drawPath(*path);
    delete path;
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

    auto size = _pBoard->boardsize();
    for (int row = 0; row < size; ++row)
    {
        for (int col = 0; col < size; ++col)
        {
            drawEllipse(_ct[row][col], Color::Red, _hex_w / 2, 0.85f);
        }
    }
}

void Canvas::renderInfo()
{
}
