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

void app::onAction(int boardsize)
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

    ui.canvas->resize(_pBoard);
}

void app::onAction5() { onAction(5); }
void app::onAction7() { onAction(7); }
void app::onAction9() { onAction(9); }
void app::onAction11() { onAction(11); }
void app::onAction13() { onAction(13); }
void app::onAction15() { onAction(15); }
void app::onAction17() { onAction(17); }
void app::onAction19() { onAction(19); }

