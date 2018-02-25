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
    setFixedSize(w * 16 / 9, w);

    QObject::connect(ui.action5, SIGNAL(triggered()), this, SLOT(changeBoardsize5()));
}

void app::paintEvent(QPaintEvent * event)
{
}

void app::changeBoardsize(int boardsize)
{
    if (boardsize <= 0)
    {
        if (!_pBoard)
            return;
        boardsize = _pBoard->boardsize();
    }
    delete _pBoard;
    _pBoard = IBoard::create(boardsize);

    ui.canvas->resize(_pBoard);
}

void app::changeBoardsize5() { changeBoardsize(5); }
void app::changeBoardsize7() { changeBoardsize(7); }
void app::changeBoardsize9() { changeBoardsize(9); }
void app::changeBoardsize11() { changeBoardsize(11); }
void app::changeBoardsize13() { changeBoardsize(13); }
void app::changeBoardsize15() { changeBoardsize(15); }
void app::changeBoardsize17() { changeBoardsize(17); }
void app::changeBoardsize19() { changeBoardsize(19); }

