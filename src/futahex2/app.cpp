#include "app.h"
#include <QPainterPath>
#include <QPainter>
#include <board.h>
using namespace std;
using namespace board;

app::app(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.centralWidget->setLayout(ui.mainLayout);
    ui.link_red->setDisplayMethod(Canvas::DisplayMethod::LinkR);
    ui.link_blue->setDisplayMethod(Canvas::DisplayMethod::LinkB);
    setFixedSize(w * 16 / 9, w);

    QObject::connect(ui.canvas, SIGNAL(clickEmptyPoint(int, int)), this, SLOT(setPiece(int, int)));
    QObject::connect(ui.action5, SIGNAL(triggered()), this, SLOT(onAction5()));
    QObject::connect(ui.action7, SIGNAL(triggered()), this, SLOT(onAction7()));
    QObject::connect(ui.action9, SIGNAL(triggered()), this, SLOT(onAction9()));
    QObject::connect(ui.action11, SIGNAL(triggered()), this, SLOT(onAction11()));
    QObject::connect(ui.action13, SIGNAL(triggered()), this, SLOT(onAction13()));
    QObject::connect(ui.action15, SIGNAL(triggered()), this, SLOT(onAction15()));
    QObject::connect(ui.action17, SIGNAL(triggered()), this, SLOT(onAction17()));
    QObject::connect(ui.action19, SIGNAL(triggered()), this, SLOT(onAction19()));
}

void app::paintEvent(QPaintEvent * event)
{
}

void app::setPiece(int row, int col)
{
    if (!_pBoard)
        return;

    (*_pBoard)(row, col) = _pBoard->color();

    updateBoard();
}

void app::changeBoardsize(int boardsize)
{
    if (boardsize <= 0)
    {
        if (!_pBoard)
            return;
        boardsize = _pBoard->boardsize();
    }

    auto & actions = ui.menuBoardsize->actions();
    for (auto action : actions)
    {
        auto text = action->text().toInt();
        action->setChecked(boardsize == text);
    }

    delete _pBoard;
    _pBoard = IBoard::create(boardsize);

    updateBoard();
}

void app::updateBoard()
{
    ui.canvas->updateBoard(_pBoard);
    ui.link_red->updateBoard();
    ui.link_blue->updateBoard();
}

void app::onAction5() { changeBoardsize(5); }
void app::onAction7() { changeBoardsize(7); }
void app::onAction9() { changeBoardsize(9); }
void app::onAction11() { changeBoardsize(11); }
void app::onAction13() { changeBoardsize(13); }
void app::onAction15() { changeBoardsize(15); }
void app::onAction17() { changeBoardsize(17); }
void app::onAction19() { changeBoardsize(19); }

