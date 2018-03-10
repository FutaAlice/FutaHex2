#include "canvas.h"
#include <cmath>
#include <iostream>
#include <map>
#include <QPainter>
#include <QPalette>
#include <QMouseEvent>
#include <board.h>

using namespace std;
using namespace board;

IBoard *Canvas::_pBoard = nullptr;

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

void Canvas::updateBoard(IBoard *pb)
{
    if (pb)
    {
        delete _pBoard;
        _pBoard = pb->copy();
    }

    this->resizeEvent(nullptr);
    this->update();
}

void Canvas::setDisplayMethod(DisplayMethod dm)
{
    _dm = dm;

    QPalette pal(palette());
    if (dm != DisplayMethod::Normal)
    {
        pal.setColor(QPalette::Background, QColor(10, 10, 10));
    }
    else
    {
    }
    setAutoFillBackground(true);
    setPalette(pal);
}

Canvas::DisplayMethod Canvas::getDisplayMethod()
{
    return _dm;
}

void Canvas::setLineWithArrow(bool arrow)
{
    _arrow = arrow;
}

bool Canvas::getLineWithArrow()
{
    return _arrow;
}

void Canvas::resizeEvent(QResizeEvent * event)
{
    if (!_pBoard)
        return;

    auto size = _pBoard->boardsize();
    assert(5 <= size && size <= 19);

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
    if (_h < 20 || _w < 20)
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
    if (DisplayMethod::Normal == _dm)
    {
        renderBorder();
        renderEmptyBoard();
        renderPieces();
        renderInfo();
    }
    else
    {
        renderLink();
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    if (!_pBoard)
        return;
    auto size = _pBoard->boardsize();

    QPointF vertex[6];
    QPolygonF hex;
    for (int row = 0; row < size; ++row)
    {
        for (int col = 0; col < size; ++col)
        {
            get_hex_vertex(vertex, _ct[row][col], _hex_w, _hex_h);
            hex.clear();
            for (auto & pt : vertex)
                hex << pt;

            if (hex.containsPoint(event->pos(), Qt::WindingFill) &&
                Color::Empty == (*_pBoard)(row, col))
                emit clickEmptyPoint(row, col);
        }
    }
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

void Canvas::renderLink()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    auto drawArrowLine = [&painter](auto start, auto end, auto length, auto degree) {
        QPointF ptArrow[2];
        auto calcVertexes = [=, &ptArrow]{
            auto pi = asin(1) * 2;
            double angle = atan2(end.y() - start.y(), end.x() - start.x()) + pi;
            ptArrow[0].setX(end.x() + length * cos(angle - degree));
            ptArrow[0].setY(end.y() + length * sin(angle - degree));
            ptArrow[1].setX(end.x() + length * cos(angle + degree));
            ptArrow[1].setY(end.y() + length * sin(angle + degree));
        };
        calcVertexes();
        painter.drawLine(start, end);
        painter.drawLine(end, ptArrow[0]);
        painter.drawLine(end, ptArrow[1]);
    };

    Color color = (_dm == DisplayMethod::LinkR) ? Color::Red : Color::Blue;
    auto & pen_color = (color == Color::Red) ? _cr : _cb;
    painter.setPen(QPen(pen_color, 1, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));

    auto size = _pBoard->boardsize();
    for (int row = 0; row < size; ++row)
    {
        for (int col = 0; col < size; ++col)
        {
            for (auto iter = _pBoard->begin(color, row * size + col);
                 iter != _pBoard->end(color, row * size + col);
                 ++iter)
            {
                auto index = *iter;
                if (index < size * size)
                {
                    int r = index / size;
                    int c = index % size;
                    if (_arrow)
                        drawArrowLine(_ct[row][col], _ct[r][c], _hex_w * 0.4, 0.15);
                    else
                        painter.drawLine(_ct[row][col], _ct[r][c]);
                    painter.drawEllipse(_ct[row][col], 1, 1);
                }
                else
                {
                    painter.drawEllipse(_ct[row][col], 3, 3);
                }
            }
        }
    }
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
            Color color = (*_pBoard)(row, col);
            if (Color::Empty == color)
                continue;
            drawEllipse(_ct[row][col], color, _hex_w / 2, 0.85f);
        }
    }
}

void Canvas::renderInfo()
{
}
